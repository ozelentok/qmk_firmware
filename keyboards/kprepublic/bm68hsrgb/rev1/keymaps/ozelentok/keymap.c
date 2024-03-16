#include QMK_KEYBOARD_H

enum custom_keycodes {
  TAB_AND_REFRESH = SAFE_RANGE,
  RED_LIGHT,
  NO_RED_LIGHT,
  PASTE_ENTER,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case TAB_AND_REFRESH:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL("\t"));
        _delay_ms(100);
        SEND_STRING(SS_LSFT("e"));
      }
      break;
    case RED_LIGHT:
      if (record->event.pressed) {
        rgb_matrix_sethsv_noeeprom(0x0, 0xFF, 0x40);
      }
      break;
    case NO_RED_LIGHT:
      if (record->event.pressed) {
        rgb_matrix_sethsv_noeeprom(0x0, 0xFF, 0x0);
      }
      break;
    case PASTE_ENTER:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL("v") SS_TAP(X_ENTER));
      }
      break;
  }
  return true;
};

/*
enum tap_dance_keycodes {
  TD_TAB_OR_TT2,
};

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_TAB_OR_TT2] = ACTION_TAP_DANCE_LAYER_TOGGLE(LT(2, KC_TAB), 2),
};
*/

/* Layout
    KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,
    KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,
    KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,                   KC_TRANSPARENT,  KC_TRANSPARENT,
    KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,                   KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,
    KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,                                                     KC_TRANSPARENT,                                    KC_TRANSPARENT   KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_65_ansi(
    KC_TRANSPARENT,  KC_1,            KC_2,            KC_3,            KC_4,            KC_5,            KC_6,            KC_7,            KC_8,            KC_9,            KC_0,            KC_MINS,         KC_EQL,          KC_BSPC,         KC_DEL,
    LT(4, KC_TAB),   KC_Q,            KC_W,            KC_E,            KC_R,            KC_T,            KC_Y,            KC_U,            KC_I,            KC_O,            KC_P,            KC_TRANSPARENT,  KC_PSCR,         KC_BSLS,         KC_INSERT,
    KC_ESC,          LGUI_T(KC_A),    LSFT_T(KC_S),    LCTL_T(KC_D),    LT(2, KC_F),     KC_G,            KC_H,            LT(3, KC_J),     RCTL_T(KC_K),    LSFT_T(KC_L),    LGUI_T(KC_SCLN), LT(4, KC_QUOT),                   KC_ENT,          KC_HOME,
    KC_LSFT,         KC_Z,            KC_X,            KC_C,            KC_V,            KC_B,            KC_N,            KC_M,            KC_COMM,         KC_DOT,          KC_SLSH,                          KC_RSFT,         KC_UP,           KC_END,
    KC_LCTL,         KC_LGUI,         KC_LALT,                                                            KC_SPC,                                            TT(1),           KC_RGUI,         KC_RCTL,         KC_LEFT,         KC_DOWN,         KC_RGHT
  ),
  [1] = LAYOUT_65_ansi(
    QK_GESC,         KC_1,            KC_2,            KC_3,            KC_4,            KC_5,            KC_6,            KC_7,            KC_8,            KC_9,            KC_0,            KC_MINS,         KC_EQL,          KC_BSPC,         KC_DEL,
    KC_TRANSPARENT,  KC_Q,            KC_W,            KC_E,            KC_R,            KC_T,            KC_Y,            KC_U,            KC_I,            KC_O,            KC_P,            KC_LBRC,         KC_RBRC,         KC_BSLS,         KC_INSERT,
    KC_TRANSPARENT,  KC_A,            KC_S,            KC_D,            KC_F,            KC_G,            KC_H,            KC_J,            KC_K,            KC_L,            KC_SCLN,         KC_QUOT,                          KC_ENT,          KC_HOME,
    KC_LSFT,         KC_Z,            KC_X,            KC_C,            KC_V,            KC_B,            KC_N,            KC_M,            KC_COMM,         KC_DOT,          KC_SLSH,                          KC_RSFT,         KC_UP,           KC_END,
    KC_LCTL,         KC_LGUI,         KC_LALT,                                                            KC_SPC,                                            KC_TRANSPARENT,  KC_RGUI,         KC_RCTL,         KC_LEFT,         KC_DOWN,         KC_RGHT
  ),
  [2] = LAYOUT_65_ansi(
    KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_ESC,          KC_ESC,          KC_ESC,          KC_TRANSPARENT,  KC_TRANSPARENT,  KC_PAUSE,        KC_PSCR,         KC_TRANSPARENT,  QK_BOOT,
    KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_DEL,          KC_BSPC,         KC_LBRC,         KC_RBRC,         KC_SLSH,         KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,
    KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_LEFT,         KC_DOWN,         KC_UP,           KC_RIGHT,        KC_QUOT,         KC_TRANSPARENT,                   KC_TRANSPARENT,  KC_TRANSPARENT,
    KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_ENTER,        KC_HOME,         KC_PGDN,         KC_PGUP,         KC_END,                           KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,
    KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,                                                     KC_TRANSPARENT,                                    KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT
  ),
  [3] = LAYOUT_65_ansi(
    KC_TRANSPARENT,  KC_CAPS,         KC_ESC,          KC_ESC,          KC_TILDE,        KC_BSLS,         KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  QK_BOOT,
    KC_TRANSPARENT,  KC_7,            KC_8,            KC_9,            KC_0,            KC_GRAVE,        KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,
    KC_TRANSPARENT,  KC_4,            KC_5,            KC_6,            KC_MINUS,        KC_EQUAL,        KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,                   KC_TRANSPARENT,  KC_TRANSPARENT,
    KC_TRANSPARENT,  KC_1,            KC_2,            KC_3,            KC_UNDS,         KC_PLUS,         KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,                   KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,
    KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,                                                     KC_TRANSPARENT,                                    KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT
  ),
  [4] = LAYOUT_65_ansi(
    KC_TRANSPARENT,  KC_F1,              KC_F2,               KC_F3,                     KC_F4,           KC_F5,           KC_F6,           KC_F7,           KC_F8,           KC_F9,           KC_F10,          KC_F11,          KC_F12,          KC_BSPC,         QK_BOOT,
    KC_TRANSPARENT,  KC_AUDIO_VOL_DOWN,  KC_AUDIO_VOL_UP,     KC_AUDIO_MUTE,             TAB_AND_REFRESH, KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  LALT(KC_SYRQ),   NO_RED_LIGHT,    RED_LIGHT,       RGB_TOG,         RGB_VAI,
    KC_TRANSPARENT,  KC_MEDIA_PLAY_PAUSE,KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK,       KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,                   KC_TRANSPARENT,  KC_TRANSPARENT,
    KC_TRANSPARENT,  KC_F13,             KC_F14,              KC_F15,                    KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,                   KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,
    KC_TRANSPARENT,  KC_TRANSPARENT,     KC_TRANSPARENT,                                                  PASTE_ENTER,                                                        KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT
  ),
};

void keyboard_post_init_user(void) {
  rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
}

bool rgb_matrix_indicators_user(void) {
  if (IS_LAYER_ON(2)) {
    rgb_matrix_set_color(34, 0x00, 0x00, 0x15);
  }
  if (IS_LAYER_ON(3)) {
    rgb_matrix_set_color(37, 0x00, 0x00, 0x15);
  }
  if (IS_LAYER_ON(4)) {
    rgb_matrix_set_color(15, 0x00, 0x00, 0x15);
  }
  if (host_keyboard_led_state().caps_lock) {
    rgb_matrix_set_color(46, 0x00, 0x15, 0x00);
  }
  return false;
}
