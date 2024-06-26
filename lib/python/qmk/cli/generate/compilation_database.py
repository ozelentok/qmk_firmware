"""Creates a compilation database for the given keyboard build.
"""

import json
import os
import re
import shlex
import shutil
from functools import lru_cache
from pathlib import Path
from typing import Dict, Iterator, List, Union

from milc import cli, MILC

from qmk.commands import find_make
from qmk.constants import QMK_FIRMWARE
from qmk.decorators import automagic_keyboard, automagic_keymap
from qmk.keyboard import keyboard_completer, keyboard_folder
from qmk.keymap import keymap_completer
from qmk.build_targets import KeyboardKeymapBuildTarget


@lru_cache(maxsize=10)
def system_libs(binary: str) -> List[Path]:
    """Find the system include directory that the given build tool uses.
    """
    cli.log.debug("searching for system library directory for binary: %s", binary)

    # Actually query xxxxxx-gcc to find its include paths.
    if binary.endswith("gcc") or binary.endswith("g++"):
        # (TODO): Remove 'stdin' once 'input' no longer causes issues under MSYS
        result = cli.run([binary, '-E', '-Wp,-v', '-'], capture_output=True, check=True, stdin=None, input='\n')
        paths = []
        for line in result.stderr.splitlines():
            if line.startswith(" "):
                paths.append(Path(line.strip()).resolve())
        return paths

    return list(Path(binary).resolve().parent.parent.glob("*/include")) if binary else []


@lru_cache(maxsize=10)
def cpu_defines(binary: str, compiler_args: str) -> List[str]:
    cli.log.debug("gathering definitions for compilation: %s %s", binary, compiler_args)
    if binary.endswith("gcc") or binary.endswith("g++"):
        invocation = [binary, '-dM', '-E']
        if binary.endswith("gcc"):
            invocation.extend(['-x', 'c'])
        elif binary.endswith("g++"):
            invocation.extend(['-x', 'c++'])
        compiler_args = shlex.split(compiler_args)
        invocation.extend(compiler_args)
        invocation.append('-')
        result = cli.run(invocation, capture_output=True, check=True, stdin=None, input='\n')
        define_args = []
        for line in result.stdout.splitlines():
            line_args = line.split(' ', 2)
            if len(line_args) == 3 and line_args[0] == '#define':
                define_args.append(f'-D{line_args[1]}={line_args[2]}')
            elif len(line_args) == 2 and line_args[0] == '#define':
                define_args.append(f'-D{line_args[1]}')
        return list(sorted(set(define_args)))
    return []


file_re = re.compile(r'printf "Compiling: ([^"]+)')
cmd_re = re.compile(r'LOG=\$\((.+?)&&')


def parse_make_n(f: Iterator[str]) -> List[Dict[str, str]]:
    """parse the output of `make -n <target>`

    This function makes many assumptions about the format of your build log.
    This happens to work right now for qmk.
    """

    state = 'start'
    this_file = None
    records = []
    for line in f:
        if state == 'start':
            m = file_re.search(line)
            if m:
                this_file = m.group(1)
                state = 'cmd'

        if state == 'cmd':
            assert this_file
            m = cmd_re.search(line)
            if m:
                # we have a hit!
                this_cmd = m.group(1)
                args = shlex.split(this_cmd)
                binary = shutil.which(args[0])
                compiler_args = set(filter(lambda x: x.startswith('-m') or x.startswith('-f'), args))
                for s in system_libs(binary):
                    args += ['-isystem', '%s' % s]
                args.extend(cpu_defines(binary, ' '.join(shlex.quote(s) for s in compiler_args)))
                new_cmd = ' '.join(shlex.quote(s) for s in args)
                records.append({"directory": str(QMK_FIRMWARE.resolve()), "command": new_cmd, "file": this_file})
                state = 'start'

    return records


def write_compilation_database(keyboard: str = None, keymap: str = None, output_path: Path = QMK_FIRMWARE / 'compile_commands.json', skip_clean: bool = False, command: List[str] = None, **env_vars) -> bool:
    # Generate the make command for a specific keyboard/keymap.
    if not command:
        from qmk.build_targets import KeyboardKeymapBuildTarget  # Lazy load due to circular references
        target = KeyboardKeymapBuildTarget(keyboard, keymap)
        command = target.compile_command(dry_run=True, **env_vars)

    if not command:
        cli.log.error('You must supply both `--keyboard` and `--keymap`, or be in a directory for a keyboard or keymap.')
        cli.echo('usage: qmk generate-compilation-database [-kb KEYBOARD] [-km KEYMAP]')
        return False

    # remove any environment variable overrides which could trip us up
    env = os.environ.copy()
    env.pop("MAKEFLAGS", None)

    # re-use same executable as the main make invocation (might be gmake)
    if not skip_clean:
        clean_command = [find_make(), "clean"]
        cli.log.info('Making clean with {fg_cyan}%s', ' '.join(clean_command))
        cli.run(clean_command, capture_output=False, check=True, env=env)

    cli.log.info('Gathering build instructions from {fg_cyan}%s', ' '.join(command))

    result = cli.run(command, capture_output=True, check=True, env=env)
    db = parse_make_n(result.stdout.splitlines())
    if not db:
        cli.log.error("Failed to parse output from make output:\n%s", result.stdout)
        return False

    cli.log.info("Found %s compile commands", len(db))

    cli.log.info(f"Writing build database to {output_path}")
    output_path.write_text(json.dumps(db, indent=4))

    return True


@cli.argument('-kb', '--keyboard', type=keyboard_folder, completer=keyboard_completer, help='The keyboard\'s name')
@cli.argument('-km', '--keymap', completer=keymap_completer, help='The keymap\'s name')
@cli.subcommand('Create a compilation database.')
@automagic_keyboard
@automagic_keymap
def generate_compilation_database(cli: MILC) -> Union[bool, int]:
    """Creates a compilation database for the given keyboard build.

    Does a make clean, then a make -n for this target and uses the dry-run output to create
    a compilation database (compile_commands.json). This file can help some IDEs and
    IDE-like editors work better. For more information about this:

        https://clang.llvm.org/docs/JSONCompilationDatabase.html
    """
    # check both config domains: the magic decorator fills in `generate_compilation_database` but the user is
    # more likely to have set `compile` in their config file.
    current_keyboard = cli.config.generate_compilation_database.keyboard or cli.config.user.keyboard
    current_keymap = cli.config.generate_compilation_database.keymap or cli.config.user.keymap

    if not current_keyboard:
        cli.log.error('Could not determine keyboard!')
    elif not current_keymap:
        cli.log.error('Could not determine keymap!')

    target = KeyboardKeymapBuildTarget(current_keyboard, current_keymap)
    return target.generate_compilation_database(skip_clean=True)
