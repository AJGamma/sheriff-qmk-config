// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "keycodes.h"
#include "keymap_us.h"
#include "modifiers.h"
#include "quantum_keycodes.h"
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

#define LMTL MT(MOD_LCTL, KC_LEFT)
#define LMTR MT(MOD_LGUI, KC_RIGHT)
#define RMTD MT(MOD_RGUI, KC_DOWN)
#define RMTU MT(MOD_RCTL, KC_UP)
#define VLTW LT(_VIM_WB, KC_W)
#define VLTE LT(_VIM_EGE, KC_E)

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_QWERTY     ] =   { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_DOWN, KC_UP)  },
    [_SYM        ] =   { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN), ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_NAV        ] =   { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT),           ENCODER_CCW_CW(KC_DEL, KC_BSPC)  },
    [_NUM        ] =   { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(QK_REP, QK_REP)  },
    [_WASD       ] =   { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_DOWN, KC_UP)  },
    [_DOUBLE     ] =   { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_DOWN, KC_UP)  },
    [_SC_TAB     ] =   { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_DOWN, KC_UP)  },
    [_REP_8      ] =   { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_DOWN, KC_UP)  },
    [_MT_QWERTY  ] =   { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_DOWN, KC_UP)  },
    [_VIM_WB     ] =   { ENCODER_CCW_CW(KC_B, KC_W), ENCODER_CCW_CW(KC_LCBR, KC_RCBR)  },
    [_VIM_EGE     ] =   { ENCODER_CCW_CW(KC_B, KC_E), ENCODER_CCW_CW(KC_DOWN, KC_UP)  },
    //                  Encoder 1                                     Encoder 2
};
#endif


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_split_70(
                               KC_ESC, KC_2, KC_3,    KC_4,    KC_5,                                                                 KC_6,     KC_7,     KC_8,     KC_9,     KC_DEL,
            KC_HOME, KC_GRAVE, KC_1,   VLTW, VLTE,    KC_R,    KC_T,                                                                 KC_Y,     KC_U,     KC_I,     KC_O,     KC_0,      KC_MINS,   KC_EQL,
            KC_END,  KC_TAB,   KC_Q,   KC_S, KC_D,    KC_F,    KC_G,                                                                 KC_H,     KC_J,     KC_K,     KC_L,     KC_P,      KC_LBRC,   KC_RBRC,
            KC_LCTL, KC_CAPS,  KC_A,   KC_X, KC_C,    KC_V,    KC_B, LT(_NUM, KC_SPC),                                    KC_ENTER,  KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SCLN,   KC_QUOT,   KC_BSLS,
                     KC_LSFT,  KC_Z,   LMTL, LMTR, KC_LCTL, KC_LGUI, KC_LALT,   LT(_NAV, KC_BSPC),   LT(_SYM, KC_ESC), KC_RSFT,   KC_RCTL,  KC_RCTL,  RMTD,  RMTU,  KC_SLSH,  KC_RSFT

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
            _______, _______, KC_F1,   _______, _______, _______, _______,                                     KC_HOME, KC_PGDN, KC_PGUP, _______, KC_END,  KC_F11,  KC_F12,
            _______, _______, _______, KC_LALT, KC_LCTL, KC_LSFT, _______,                                     KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT,  KC_MPRV, _______, _______,
            _______, _______, KC_LGUI, _______, _______, _______, _______, _______,                   _______, _______, KC_MUTE, KC_VOLD, KC_VOLU, KC_MPLY, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, KC_ESC,  _______, _______, _______, _______, _______, KC_MNXT, _______
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
                          KC_NO, KC_NO,     KC_NO, KC_NO, KC_NO,                             KC_NO, KC_NO,          KC_NO, KC_NO, KC_NO,
            KC_NO, KC_NO, KC_NO, TO(_WASD), KC_NO, KC_NO, KC_NO,                             KC_NO, KC_NO,          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
            KC_NO, KC_NO, TO(_QWERTY),      KC_NO, KC_NO, KC_NO, KC_NO,                      KC_NO, TO(_MT_QWERTY), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
            KC_NO, KC_NO, KC_NO,            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,               KC_NO, KC_NO,          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                   KC_NO, KC_NO,            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO

    ),
    [_SC_TAB] = LAYOUT_split_70(
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

    ),
    [_REP_8] = LAYOUT_split_70(
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

    ),
    [_MT_QWERTY] = LAYOUT_split_70(
                               KC_ESC,             KC_2,             KC_3,    KC_4,    KC_5,                                                                 KC_6,     KC_7,     KC_8,     KC_9,     KC_DEL,
            KC_HOME, KC_GRAVE, KC_1,               VLTW,             VLTE,    KC_R,    KC_T,                                                                 KC_Y,     KC_U,     KC_I,     KC_O,     KC_0,      KC_MINS,   KC_EQL,
            KC_END,  KC_TAB,   KC_Q, MT(MOD_LSFT, KC_S), MT(MOD_LSFT, KC_D), MT(MOD_LSFT, KC_F),    KC_G,                                KC_H,     MT(MOD_LSFT, KC_J),     MT(MOD_LSFT, KC_K),     MT(MOD_LSFT, KC_L),     KC_P,      KC_LBRC,   KC_RBRC,
            KC_LCTL, KC_CAPS,  MT(MOD_LGUI, KC_A), KC_X,             KC_C,    KC_V,    KC_B, LT(_NUM, KC_SPC),                                    KC_ENTER,  KC_N,     KC_M,     MT(MOD_LSFT, KC_COMM),  KC_DOT,   KC_SCLN,   KC_QUOT,   KC_BSLS,
                     KC_LSFT,  KC_Z,               LMTL,             LMTR, KC_LCTL, KC_LGUI, KC_LALT,   LT(_NAV, KC_BSPC),   LT(_SYM, KC_ESC), KC_RSFT,   KC_RCTL,  KC_RCTL,  RMTD,  RMTU,  KC_SLSH,  KC_RSFT

    ),
    [_VIM_WB] = LAYOUT_split_70(

                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

    ),
    [_VIM_EGE] = LAYOUT_split_70(

                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS

    ),
};
