#include QMK_KEYBOARD_H
#include "rgblight.h"

#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 3
#define _APEX 4

#define __ KC_TRNS
#define ___ KC_TRNS
#define ____ KC_TRNS
#define _____ KC_TRNS
#define ______ KC_TRNS

bool is_lalt_pressed = false;
bool is_tab_pressed = false;
bool is_space_pressed = false;
bool is_alt_tab_active = false;
/* bool is_apex_lalt_active = false; */

// DBL_TAP_ESC
uint16_t escape_timer = 0;
bool escape_tapped = false;

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  APEX,
  ALT_TAB,
  DBL_TAP_ESC
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                               KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, RCTL_T(KC_QUOT),
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_HOME,          KC_END,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    KC_LGUI, LOWER,   KC_ENT,                    KC_SPC,  RAISE,   KC_RALT
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_LOWER] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_TILD, LALT(KC_1), LALT(KC_2), LALT(KC_3), LALT(KC_4), LALT(KC_5),             LALT(KC_6), LALT(KC_7), LALT(KC_8), LALT(KC_9), LALT(KC_0), KC_DEL,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     ALT_TAB, LALT(KC_Q),  ___, LALT(KC_E), ____, _______,                            ____, LALT(KC_U), LALT(KC_I), ____, _______, KC_PLUS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_DEL,  _______, KC_LEFT, KC_RGHT, KC_UP,   KC_LBRC,                            KC_RBRC, _______, _______, _____, LALT(KC_SCLN), KC_PIPE,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     BL_STEP, _______, _______, LALT(KC_C), KC_DOWN, KC_LCBR, KC_LPRN,       KC_RPRN, KC_RCBR, _______, _____, LALT(KC_DOT), _____, KC_EQL,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, ______,                    ______,  _______, KC_P0
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_RAISE] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_F12,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     RGB_TOG, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                            KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     RGB_MOD, KC_MPRV, KC_MNXT, KC_VOLU, KC_PGUP, KC_UNDS,                            KC_EQL,  KC_HOME, RGB_HUI, RGB_SAI, RGB_VAI, KC_BSLS,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_MUTE, KC_MSTP, KC_MPLY, KC_VOLD, KC_PGDN, KC_MINS, KC_LPRN,          _______, KC_PLUS, KC_END,  RGB_HUD, RGB_SAD, RGB_VAD, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_ADJUST] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     APEX,    _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_APEX] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
   DBL_TAP_ESC, KC_1,  KC_2,    KC_3,    KC_4,    KC_F1,                              QWERTY,  QWERTY,  QWERTY,  QWERTY,  QWERTY,  QWERTY,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               QWERTY,  QWERTY,  QWERTY,  QWERTY,  QWERTY,  QWERTY,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                               QWERTY,  QWERTY,  QWERTY,  QWERTY,  QWERTY,  QWERTY,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_M,             QWERTY,  QWERTY,  QWERTY,  QWERTY,  QWERTY,  QWERTY,  QWERTY,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                  KC_LALT, KC_LALT, KC_SPC,                 QWERTY,  QWERTY,  QWERTY
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  )
};

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
   if (index == 0) {
      if (clockwise) {
         tap_code(KC_PGDN);
      } else {
         tap_code(KC_PGUP);
      }
   } else if (index == 1) {
      if (clockwise) {
         tap_code(KC_VOLU);
      } else {
         tap_code(KC_VOLD);
      }
   }
   return false;
}
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
   if (is_alt_tab_active) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
   }

   // Store the current RGB mode to restore it later
   static uint8_t saved_rgb_mode;

   // Apex layer handling
   if (layer_state_cmp(state, _APEX)) {
      // Save the current RGB mode
      saved_rgb_mode = rgblight_get_mode();
      // Set the RGB color to red and disable any animations
      rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
      rgblight_sethsv(0, 255, 255);
   } else {
      // Restore the previous RGB mode
      rgblight_mode(saved_rgb_mode);
   }

   return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (IS_LAYER_ON(_APEX)) {
    switch (keycode) {
      case KC_LALT:
        is_lalt_pressed = record->event.pressed;
        break;
      case KC_TAB:
        if (is_lalt_pressed) {
          is_tab_pressed = record->event.pressed;
          return false;
        }
        break;
      case KC_SPC:
        if (is_lalt_pressed) {
          is_space_pressed = record->event.pressed;
          return false;
        }
        break;
    }
  }

  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case APEX:
      if (record->event.pressed) {
         layer_invert(_APEX);
         if (layer_state_is(_APEX)) {
            rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
            rgblight_sethsv(0, 255, 255); // Set to red color
         } else {
            rgblight_mode(1); // Set to the default RGB lighting mode
            is_lalt_pressed = false;
            is_tab_pressed = false;
            is_space_pressed = false;
         }
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
   case ADJUST:
      if (record->event.pressed) {
         layer_on(_ADJUST);
      } else {
         layer_off(_ADJUST);
      }
      return false;
      break;
   case ALT_TAB:
      if (record->event.pressed) {
         if (!is_alt_tab_active) {
            is_alt_tab_active = true;
            register_code(KC_LALT);
         }
         register_code(KC_TAB);
      } else {
         unregister_code(KC_TAB);
      }
      break;
   case DBL_TAP_ESC:
      if (record->event.pressed) {
         if (escape_tapped && timer_elapsed(escape_timer) < TAPPING_TERM) {
            register_code(KC_ESC);
            unregister_code(KC_ESC);
            escape_tapped = false;
         } else {
            escape_tapped = true;
            escape_timer = timer_read();
         }
      }
      return false;
      break;
  }
  return true;
}
