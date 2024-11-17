// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdint.h>
#include "keycodes.h"
#include "keymap_us.h"
#include "modifiers.h"
#include "quantum_keycodes.h"
#include QMK_KEYBOARD_H

#define _QWERTY      0
#define _SYM         1
#define _NAV         2
#define _NUM         3
#define _SC_TAB      4
#define _VIM_WB      5
#define _VIM_EGE     6
#define _MOUSE       7
#define _STRANGE     8
#define _WASD        9
#define _DOUBLE      10

#define LMTL MT(MOD_LCTL, KC_LEFT)
#define LMTR MT(MOD_LGUI, KC_RIGHT)
#define RMTD MT(MOD_RGUI, KC_DOWN)
#define RMTU MT(MOD_RCTL, KC_UP)
#define VLTW LT(_VIM_WB, KC_W)
#define VLTE LT(_VIM_EGE, KC_E)

#define CUSTOM_TAP_LAYER_TIMEOUT 1000

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
     [_QWERTY     ] =  { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_DOWN, KC_UP)            },
     [_SYM        ] =  { ENCODER_CCW_CW(KC_BSPC, KC_DEL),           ENCODER_CCW_CW(KC_BSPC, KC_DEL) },
     [_NAV        ] =  { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN) },
     [_NUM        ] =  { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(QK_REP, QK_REP)            },
     [_WASD       ] =  { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_DOWN, KC_UP)            },
     [_DOUBLE     ] =  { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_DOWN, KC_UP)            },
     [_SC_TAB     ] =  { ENCODER_CCW_CW(RSFT(KC_TAB), KC_TAB), ENCODER_CCW_CW(RSFT(KC_TAB), KC_TAB)   },
     [_VIM_WB     ] =  { ENCODER_CCW_CW(KC_B, KC_W), ENCODER_CCW_CW(KC_RCBR, KC_LCBR)                 },
    [_VIM_EGE     ] =  { ENCODER_CCW_CW(KC_B, KC_E), ENCODER_CCW_CW(KC_DOWN, KC_UP)                  },
      [_MOUSE     ] =  { ENCODER_CCW_CW(MS_LEFT, MS_RGHT), ENCODER_CCW_CW(MS_DOWN, MS_UP)                    },
      [_STRANGE     ] =  { ENCODER_CCW_CW(MS_LEFT, MS_RGHT), ENCODER_CCW_CW(MS_DOWN, MS_UP)                    },
    //                  Encoder 1                                     Encoder 2
};
#endif


bool is_rep_8_active = false;
bool is_vim_wb_active = false;
bool is_vim_ege_active = false;
bool is_sc_tab_active = false;

uint16_t is_rep_8_timer = 0;
uint16_t is_vim_wb_timer = 0;
uint16_t is_vim_ege_timer = 0;
uint16_t is_sc_tab_timer = 0;






const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_split_70(
                               KC_ESC, KC_2, KC_3, KC_4,             KC_5,                                                                 KC_6, KC_7,       KC_8,     KC_9,    KC_DEL,
            KC_HOME, KC_GRAVE, KC_1,   VLTW, VLTE, KC_R,             KC_T,                                                                 KC_Y, KC_U,       KC_I,     KC_O,    KC_0,    KC_MINS,   KC_EQL,
            KC_END,  KC_TAB,   KC_Q,   KC_S, KC_D, KC_F,             KC_G,                                                                 KC_H, KC_J,       KC_K,     KC_L,    KC_P,    KC_LBRC,   KC_RBRC,
            KC_LCTL, KC_CAPS,  KC_A,   KC_X, KC_C, LT(_MOUSE, KC_V), KC_B, LT(_NUM, KC_SPC),                             KC_ENTER,  KC_N,  KC_M, KC_COMM,    KC_DOT,   KC_SCLN, KC_QUOT, KC_BSLS,
                     KC_LSFT,  KC_Z,   LMTL, LMTR, KC_LCTL,          KC_LGUI, KC_LALT,   LT(_NAV, KC_ESC),   LT(_SYM, KC_BSPC),  KC_RSFT,  KC_RCTL, KC_RCTL, RMTD,     RMTU,    KC_SLSH, KC_RSFT

    ),
    [_SYM] = LAYOUT_split_70(
                              _______, KC_F2,   KC_F3,   KC_F4,   KC_F5,                                           KC_F6,   KC_F7,   KC_F8,   KC_F9,   _______,
            _______, _______, KC_F1,   KC_TILD, KC_MINS, KC_EQL,  _______,                                         _______, KC_LCBR, KC_RCBR, KC_LBRC, KC_F10,  KC_F11,  KC_F12,
            _______, KC_GRV,  KC_GRV,  KC_AT, KC_HASH, KC_DOLLAR, KC_PERC,                                         KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RBRC, _______, _______,
            _______, KC_CAPS, KC_EXLM, KC_LCTL, _______, _______, _______,     _______,                   _______, KC_BSLS, KC_QUOT, KC_DQT,  _______, KC_RPRN, _______, _______,
                     _______, _______, _______, _______, _______, MO(_DOUBLE), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [_NAV] = LAYOUT_split_70(

                              _______, KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_F6,   KC_F7,   KC_F8,   KC_F9,   _______,
            _______, _______, KC_F1,   _______, _______, _______, _______,                                     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_END,  KC_F11,  KC_F12,
            _______, _______, _______, KC_LALT, KC_LCTL, KC_LSFT, _______,                                     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_MPRV, _______, _______,
            _______, _______, KC_LGUI, _______, _______, _______, _______, _______,                   _______, _______, KC_MUTE, KC_VOLD, KC_VOLU, KC_MPLY, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, KC_ESC,  _______, _______, _______, KC_BRID, KC_BRIU, KC_MNXT, _______
    ),
    [_NUM] = LAYOUT_split_70(
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_PLUS, KC_7,    KC_8,    KC_9,    KC_BSPC, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, _______, KC_LALT, KC_LCTL, KC_LSFT, _______,                                     KC_MINS, KC_4,    KC_5,    KC_6,    KC_ASTR, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_LGUI, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_ENT,  KC_TRNS, KC_1,    KC_2,    KC_3,    KC_DOT,  KC_TRNS, KC_TRNS,
                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_RSFT, KC_0,    KC_TRNS, KC_0,    KC_DOT,  KC_SLSH, KC_TRNS
    ),
    [_WASD] = LAYOUT_split_70(
                               KC_1,   KC_2,  KC_3,    KC_4,    KC_5,                                                   KC_6,     KC_7,     KC_8,     KC_9,     KC_DEL,
            KC_ESC,  KC_TAB,   KC_Q,   KC_W,  KC_E,    KC_R,    KC_T,                                                   KC_Y,     KC_U,     KC_I,     KC_O,     KC_0,      KC_MINS,   KC_EQL,
            KC_GRV,  KC_RSFT,  KC_A,   KC_S,  KC_D,    KC_F,    KC_G,                                                   KC_H,     KC_J,     KC_K,     KC_L,     KC_P,      KC_LBRC,   KC_RBRC,
            KC_LCTL, KC_LCTL,  KC_Z,   KC_X,  KC_C,    KC_V,    KC_B, KC_SPC,                                KC_ENTER,  KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SCLN,   KC_QUOT,   KC_BSLS,
                     KC_LSFT,  KC_NO,  KC_NO, KC_NO, KC_LCTL, KC_SPC, KC_LALT,   KC_ESC,   LT(_SYM, KC_ESC), KC_RSFT,   KC_RCTL,  KC_RCTL,  RMTD,  RMTU,  KC_SLSH,  KC_RSFT

    ),
    [_DOUBLE] = LAYOUT_split_70(
                          KC_NO, KC_NO,     KC_NO, KC_NO, KC_NO,                             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
            KC_NO, KC_NO, KC_NO, DF(_WASD), KC_NO, KC_NO, KC_NO,                             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
            KC_NO, KC_NO, DF(_QWERTY),      KC_NO, KC_NO, KC_NO, KC_NO,                      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
            KC_NO, KC_NO, KC_NO,            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,               KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                   KC_NO, KC_NO,            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO

    ),
    [_SC_TAB] = LAYOUT_split_70(
                              _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______

    ),
    // [_REP_8] = LAYOUT_split_70(
    //                           _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______,
    //         _______, _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______, _______,
    //         _______, _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______, _______,
    //         _______, _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, _______, _______,
    //                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    //
    // ),
    // [_MT_QWERTY] = LAYOUT_split_70(
    //                            KC_ESC,             KC_2,             KC_3,    KC_4,    KC_5,                                                                 KC_6,     KC_7,     KC_8,     KC_9,     KC_DEL,
    //         KC_HOME, KC_GRAVE, KC_1,               VLTW,             VLTE,    KC_R,    KC_T,                                                                 KC_Y,     KC_U,     KC_I,     KC_O,     KC_0,      KC_MINS,   KC_EQL,
    //         KC_END,  KC_TAB,   KC_Q, MT(MOD_LALT, KC_S), MT(MOD_LCTL, KC_D), MT(MOD_LSFT, KC_F),    KC_G,                                KC_H,     MT(MOD_RSFT, KC_J),     MT(MOD_RCTL, KC_K),     MT(MOD_RALT, KC_L),   KC_P,     KC_LBRC,   KC_RBRC,
    //         KC_LCTL, KC_CAPS,  MT(MOD_LGUI, KC_A), KC_X,             KC_C,    KC_V,    KC_B, LT(_NUM, KC_SPC),                                    KC_ENTER,  KC_N,     KC_M,     MT(MOD_RGUI, KC_COMM),  KC_DOT,   KC_SCLN,   KC_QUOT,   KC_BSLS,
    //                  KC_LSFT,  KC_Z,               LMTL,             LMTR, KC_LCTL, KC_LGUI, KC_LALT,   LT(_NAV, KC_BSPC),   LT(_SYM, KC_ESC), KC_RSFT,   KC_RCTL,  KC_RCTL,  RMTD,  RMTU,  KC_SLSH,  KC_RSFT
    //
    // ),
    [_VIM_WB] = LAYOUT_split_70(

                              _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______

    ),
    [_STRANGE] = LAYOUT_split_70(

                              _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______

    ),
    [_VIM_EGE] = LAYOUT_split_70(

                              _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______

    ),
    [_MOUSE] = LAYOUT_split_70(

                              _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, MS_ACL1, MS_ACL2, _______, _______,                                     MS_LEFT, MS_DOWN, MS_UP,   MS_RGHT, _______, _______, _______,
            _______, _______, MS_ACL0, _______, _______, _______, _______, _______,                   _______, _______, MS_BTN1, MS_BTN2, _______, _______, _______, _______,
                     _______, _______, _______, _______, MS_BTN1, _______, _______, _______, _______, _______, _______, MS_BTN2, _______, _______, _______, _______

    ),
};


bool encoder_update_user(uint8_t index, bool clockwise) {
    layer_state_t current_layer_state = get_highest_layer(layer_state);

    if (index == 0) { /* First encoder */
        if (!clockwise) {
            switch (get_highest_layer(layer_state)) {
                case _QWERTY:
                    tap_code(KC_LEFT); tap_code(KC_LEFT); tap_code(KC_LEFT); tap_code(KC_LEFT);
                    tap_code(KC_LEFT); tap_code(KC_LEFT); tap_code(KC_LEFT); tap_code(KC_LEFT);
                    break;
                default:
                    return true;
            }
            return false;

        } else {
            switch (get_highest_layer(layer_state)) {
                case _QWERTY:
                    tap_code(KC_RIGHT); tap_code(KC_RIGHT); tap_code(KC_RIGHT); tap_code(KC_RIGHT);
                    tap_code(KC_RIGHT); tap_code(KC_RIGHT); tap_code(KC_RIGHT); tap_code(KC_RIGHT);
                    break;
                default:
                    return true;
            }
            return false;
        }
    } else if (index == 1) { /* Second encoder */
        if (!clockwise) {

            switch (get_highest_layer(layer_state)) {
                case _QWERTY:
                    tap_code(KC_DOWN); tap_code(KC_DOWN); tap_code(KC_DOWN); tap_code(KC_DOWN);
                    tap_code(KC_DOWN); tap_code(KC_DOWN); tap_code(KC_DOWN); tap_code(KC_DOWN);
                    break;
                default:
                    return true;
            }
            return false;
        } else {
            switch (get_highest_layer(layer_state)) {
                case _QWERTY:
                    tap_code(KC_UP); tap_code(KC_UP); tap_code(KC_UP); tap_code(KC_UP);
                    tap_code(KC_UP); tap_code(KC_UP); tap_code(KC_UP); tap_code(KC_UP);
                    break;
                default:
                    return true;
            }
            return false;

        }
    }
    return true;
}



bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_8:
      if (record->event.pressed) {
        // Do something when pressed
      } else {
        // Do something else when release
      }
      return false; // Skip all further processing of this key
    case KC_ENTER:
      // Play a tone when enter is pressed
      if (record->event.pressed) {
      }
      return true; // Let QMK send the enter press/release events
    default:
      return true; // Process all other keycodes normally
  }
}
