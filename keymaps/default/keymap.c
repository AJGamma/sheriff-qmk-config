// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#define _QWERTY      0
#define _SYM         1
#define _NAV         2
#define _NUM         3
#define _WASD        4
#define _DOUBLE      5
#define _SC_TAB      6
#define _REP_8       7
#define _MT_QWERTY   8
#define _VIM_WB      9
#define _VIM_EGE     10



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_split_70(
                               KC_ESC, KC_2,    KC_3,    KC_4,    KC_5,                                                      KC_6,     KC_7,     KC_8,     KC_9,     KC_DEL,
            KC_HOME, KC_GRAVE, KC_1,   KC_W,    KC_E,    KC_R,    KC_T,                                                      KC_Y,     KC_U,     KC_I,     KC_O,     KC_0,      KC_MINS,   KC_EQL,
            KC_END,  KC_TAB,   KC_Q,   KC_S,    KC_D,    KC_F,    KC_G,                                                      KC_H,     KC_J,     KC_K,     KC_L,     KC_P,      KC_LBRC,   KC_RBRC,
            KC_LCTL, KC_CAPS,  KC_A,   KC_X,    KC_C,    KC_V,    KC_B,    LT(_NUM, KC_SPC),                      KC_ENTER,  KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SCLN,   KC_QUOT,   KC_BSLS,
                     KC_LSFT,  KC_Z,   KC_LSFT, KC_DOWN, KC_LCTL, KC_LGUI, KC_LALT,   LT(_NAV, KC_BSPC),            KC_ESC, KC_RSFT,   KC_RCTL,  KC_RCTL,  KC_RGUI,  KC_RCTL,  KC_SLSH,  KC_RSFT

    ),
    [_SYM] = LAYOUT_split_70(
                               KC_ESC, KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,     KC_7,     KC_8,     KC_9,     KC_DEL,
            KC_HOME, KC_GRAVE, KC_1,   KC_W,    KC_E,    KC_R,    KC_T,                                        KC_Y,     KC_U,     KC_I,     KC_O,     KC_0,      KC_MINS,   KC_EQL,
            KC_END,  KC_TAB,   KC_Q,   KC_S,    KC_D,    KC_F,    KC_G,                                        KC_H,     KC_J,     KC_K,     KC_L,     KC_P,      KC_LBRC,   KC_RBRC,
            KC_LCTL, KC_CAPS,  KC_A,   KC_X,    KC_C,    KC_V,    KC_B,    KC_SPC,                    KC_ENTER,  KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SCLN,   KC_QUOT,   KC_BSLS,
                     KC_LSFT,  KC_Z,   KC_LSFT, KC_DOWN, KC_LCTL, KC_LGUI, KC_LALT,   KC_BSPC,  KC_ESC, KC_RSFT,  KC_RCTL,  KC_RCTL,  KC_RGUI,  KC_RCTL,  KC_SLSH,  KC_RSFT

    ),
    [_NAV] = LAYOUT_split_70(
                               KC_ESC, KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,     KC_7,     KC_8,     KC_9,     KC_DEL,
            KC_HOME, KC_GRAVE, KC_1,   KC_W,    KC_E,    KC_R,    KC_T,                                        KC_Y,     KC_U,     KC_I,     KC_O,     KC_0,      KC_MINS,   KC_EQL,
            KC_END,  KC_TAB,   KC_Q,   KC_S,    KC_D,    KC_F,    KC_G,                                        KC_H,     KC_J,     KC_K,     KC_L,     KC_P,      KC_LBRC,   KC_RBRC,
            KC_LCTL, KC_CAPS,  KC_A,   KC_X,    KC_C,    KC_V,    KC_B,    KC_SPC,                    KC_ENTER,  KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SCLN,   KC_QUOT,   KC_BSLS,
                     KC_LSFT,  KC_Z,   KC_LSFT, KC_DOWN, KC_LCTL, KC_LGUI, KC_LALT,   KC_BSPC,  KC_ESC, KC_RSFT,  KC_RCTL,  KC_RCTL,  KC_RGUI,  KC_RCTL,  KC_SLSH,  KC_RSFT

    ),
};
