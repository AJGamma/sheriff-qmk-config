// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdbool.h>
#include <stdint.h>
#include "action_util.h"
#include "my_local_passwd.h"
#include "action.h"
#include "action_layer.h"
#include "keycodes.h"
#include "keymap_us.h"
#include "matrix.h"
#include "modifiers.h"
#include "quantum.h"
#include "quantum_keycodes.h"
#include "quantum_keycodes_legacy.h"
#include "repeat_key.h"
#include "send_string_keycodes.h"
#include "timer.h"
#include QMK_KEYBOARD_H

#define MY_TAP_TIME 7
#define MY_DELAY_TIME 7
#define STRING_DELAY 20


#define CHECK_AND_TAP(key, is_held, is_active, timer, original_key) \
    if (record->event.pressed && is_held && !is_active) {           \
        is_held = false;                                            \
        timer   = timer_read();                                     \
        tap_code_delay(key, MY_TAP_TIME);                           \
        SEND_STRING(SS_DELAY(10));                                  \
        tap_code_delay(original_key, MY_TAP_TIME);                  \
        return false;                                               \
    }

#define PROCESS_SCROLL_LAYER(key, is_held, is_active, timer, mods) \
    case key:                                                      \
        if (record->event.pressed) {                               \
            is_held = true;                                        \
            timer   = timer_read();                                \
            mods    = get_mods();                                  \
        } else {                                                   \
            if (is_held) {                                         \
                add_weak_mods(mods);                               \
                tap_code16(key);                                   \
                unregister_weak_mods(mods);                        \
            }                                                      \
            is_held   = false;                                     \
            is_active = false;                                     \
            timer     = timer_read();                              \
        }                                                          \
        return false; // Skip all further processing of this key

// // define macro for this snippet:
//     if (is_8_held) {
//         if(timer_elapsed(is_rep_8_timer) > CUSTOM_TAP_LAYER_TIMEOUT) {
//             is_rep_8_active = true;
//         }
//     }
#define TRY_TO_ACTIVATE_LAYER_KEY(is_held, timer, is_active, timeout) \
    if (is_held) {                                                    \
        if (timer_elapsed(timer) > timeout) {                         \
            is_active = true;                                         \
            is_held   = false;                                        \
            timer     = timer_read();                                 \
        }                                                             \
    }

#define ACTIVATE_LAYER_KEY(is_held, is_active, timer) \
    if (is_held) {                                    \
        is_active = true;                             \
        is_held   = false;                            \
        timer     = timer_read();                     \
    }

#define DECLARE_SCROLL_LAYER_STATE(keycode, layer)   \
    bool     is_##layer##_active            = false; \
    bool     is_##keycode##_held            = false; \
    uint16_t last_time_##keycode##_released = 0;     \
    uint16_t is_##layer##_timer             = 0;     \
    uint8_t  key_##keycode##_mods           = 0;

#define _QWERTY 0
#define _WASD 1
#define _WASD1 2
#define _SINGLE 3 // single handed layer, for stuff like 3D modeling
#define _SYM 4
#define _NAV 5
#define _NUM 6
#define _MOUSE 7
#define _DOUBLE 8

#define LMTL MT(MOD_LCTL, KC_LEFT)
#define LMTR MT(MOD_LGUI, KC_RIGHT)
#define RMTD MT(MOD_RGUI, KC_DOWN)
#define RMTU MT(MOD_RCTL, KC_UP)

///////////////////////////////////////////////////////
///////////////////Double layer macros/////////////////
////////////Just for better formatting/////////////////
///////////////////////////////////////////////////////
#define a_MDA MT(MOD_LCTL, KC_A)
#define a_MDS MT(MOD_LSFT, KC_S)
#define a_MDD MT(MOD_LSFT, KC_D)
#define a_MDF MT(MOD_LCTL, KC_F)
#define a_MDJ MT(MOD_LCTL, KC_J)
#define a_MDK MT(MOD_LSFT, KC_K)
#define a_MDL MT(MOD_LSFT, KC_L)
#define a_MDSEMI MT(MOD_LCTL, KC_SCLN)

#define CUSTOM_TAP_LAYER_TIMEOUT 500
#define SCROLL_MOUSE_DELAY 30

#define DELETE_TAPPING_TERM 100

const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
    {{6, 5}, {5, 5}, {4, 5}, {3, 5}, {2, 5}, {1, 5}, {0, 5}}, {{6, 6}, {5, 6}, {4, 6}, {3, 6}, {2, 6}, {1, 6}, {0, 6}}, {{6, 7}, {5, 7}, {4, 7}, {3, 7}, {2, 7}, {1, 7}, {0, 7}}, {{6, 8}, {5, 8}, {4, 8}, {3, 8}, {2, 8}, {1, 8}, {0, 8}}, {{6, 9}, {5, 9}, {4, 9}, {3, 9}, {2, 9}, {1, 9}, {0, 9}}, {{6, 0}, {5, 0}, {4, 0}, {3, 0}, {2, 0}, {1, 0}, {0, 0}}, {{6, 1}, {5, 1}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1}}, {{6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2}}, {{6, 3}, {5, 3}, {4, 3}, {3, 3}, {2, 3}, {1, 3}, {0, 3}}, {{6, 4}, {5, 4}, {4, 4}, {3, 4}, {2, 4}, {1, 4}, {0, 4}},
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_QWERTY] = {ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_DOWN, KC_UP)}, [_SINGLE] = {ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_DOWN, KC_UP)}, [_SYM] = {ENCODER_CCW_CW(KC_BSPC, KC_DEL), ENCODER_CCW_CW(KC_BSPC, KC_DEL)}, [_NAV] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)}, [_NUM] = {ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(QK_REP, QK_REP)}, [_WASD] = {ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_DOWN, KC_UP)}, [_WASD1] = {ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_DOWN, KC_UP)}, [_DOUBLE] = {ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_DOWN, KC_UP)}, [_MOUSE] = {ENCODER_CCW_CW(MS_LEFT, MS_RGHT), ENCODER_CCW_CW(MS_DOWN, MS_UP)},
    //                  Encoder 1                                     Encoder 2
};
#endif

// #if defined(SWAP_HANDS_ENABLE) && defined(ENCODER_MAP_ENABLE)
// const uint8_t PROGMEM encoder_hand_swap_config[NUM_ENCODERS] = { 1, 0 };
// #endif

bool is_scroll_layer_deactivated = false;

DECLARE_SCROLL_LAYER_STATE(w, vim_wb);
DECLARE_SCROLL_LAYER_STATE(e, vim_ege);
DECLARE_SCROLL_LAYER_STATE(g, vim_g);
DECLARE_SCROLL_LAYER_STATE(tab, sc_tab);
DECLARE_SCROLL_LAYER_STATE(u, strange);
DECLARE_SCROLL_LAYER_STATE(y, redo_y);
DECLARE_SCROLL_LAYER_STATE(z, undo_z);

bool     is_rep_8_active         = false;
bool     is_8_held               = false;
bool     is_i_held               = false;
bool     is_pgup_held            = false;
uint16_t last_time_8_released    = 0;
uint16_t last_time_i_released    = 0;
uint16_t last_time_pgup_released = 0;
uint16_t is_rep_8_timer          = 0;
uint8_t  key_8_mods              = 0;
uint8_t  key_i_mods              = 0;
uint8_t  key_pgup_mods           = 0;

// clang-format off

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_split_70(
                               KC_ESC, KC_2, KC_3, KC_4,              KC_5,                                                                KC_6,  KC_7,    KC_8,    KC_9,   KC_DEL,
            KC_HOME, KC_GRAVE, KC_1,   KC_W, KC_E, KC_R,              KC_T,                                                                KC_Y,  KC_U,    KC_I,    KC_O,   KC_0,    KC_MINS, KC_EQL,
            KC_END,  KC_TAB,   KC_Q,   KC_S, KC_D, KC_F,              KC_G,                                                                KC_H,  KC_J,    KC_K,    KC_L,   KC_P,    KC_LBRC, KC_RBRC,
            KC_LCTL, KC_CAPS,  KC_A,   KC_X, KC_C, LT(_MOUSE, KC_V),  KC_B, LT(_NUM, KC_SPC),                                    KC_ENTER, KC_N,  KC_M,    KC_COMM, KC_DOT, KC_SCLN, KC_QUOT, KC_BSLS,
                     KC_LSFT,  KC_Z,   LMTL, LMTR, KC_LCTL,           KC_LGUI, KC_LALT,  LT(_NAV, KC_ESC), LT(_SYM, KC_BSPC),  KC_RSFT,  KC_RCTL, KC_RCTL, RMTD,    RMTU,   KC_SLSH, KC_RSFT

    ),
    [_SINGLE] = LAYOUT_split_70(
                          DF(_QWERTY),  KC_2,  KC_3,  KC_4,    KC_5,                                                                                      KC_6,     KC_7,  KC_8,    KC_9,    KC_DEL,
            KC_HOME, KC_GRAVE, KC_1,    KC_W,  KC_E,  KC_R,    KC_T,                                                                                      KC_Y,     KC_U,  KC_I,    KC_O,    KC_0,     KC_MINS, KC_EQL,
            KC_END,  KC_TAB,   KC_Q,    a_MDS, a_MDD, a_MDF,   KC_G,                                                                                      KC_H,     a_MDJ, a_MDK,   a_MDL,   KC_P,     KC_LBRC, KC_RBRC,
            KC_LCTL, KC_CAPS,  a_MDA,   KC_X,  KC_C,  KC_V,    KC_B,          KC_ENT,                                                             KC_ENT, KC_N,     KC_M,  KC_COMM, KC_DOT,  a_MDSEMI, KC_QUOT, KC_BSLS,
                     KC_LSFT,  KC_Z,    LMTL,  LMTR,  KC_LCTL, SH_T(KC_SPC),  MT(MOD_LALT, KC_BSPC), KC_ESC, KC_BSPC, MT(MOD_LSFT, KC_ESC), SH_T(KC_SPC), KC_RCTL,  RMTD,    RMTU,    KC_SLSH,  KC_RSFT

    ),
    [_SYM] = LAYOUT_split_70(
                              _______, KC_F2,   KC_F3,   KC_F4,   KC_F5,                                           KC_F6,   KC_F7,   KC_F8,   KC_F9,   _______,
            _______, _______, KC_F1,   KC_TILD, KC_MINS, KC_EQL,  _______,                                         _______, KC_LCBR, KC_RCBR, KC_LBRC, KC_F10,  KC_F11,  KC_F12,
            _______, KC_GRV,  KC_GRV,  KC_AT, KC_HASH, KC_DOLLAR, KC_PERC,                                         KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RBRC, _______, _______,
            _______, KC_CAPS, KC_EXLM, KC_LCTL, KC_UNDS, KC_PLUS, _______,     _______,                   _______, KC_BSLS, KC_QUOT, KC_DQT,  _______, KC_RPRN, _______, _______,
                     _______, _______, _______, _______, _______, MO(_DOUBLE), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [_NAV] = LAYOUT_split_70(

                              _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______,
            _______, _______, _______, _______, KC_EQL,  KC_PLUS, _______,                                     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_END,  KC_F11,  KC_F12,
            _______, _______, _______, KC_LALT, KC_LCTL, KC_LSFT, _______,                                     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_MPRV, _______, _______,
            _______, _______, KC_LGUI, _______, KC_UNDS, KC_MINS, _______, _______,                   _______, _______, KC_MUTE, KC_VOLD, KC_VOLU, KC_MPLY, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, KC_ESC,  _______, _______, _______, KC_BRID, KC_BRIU, KC_MNXT, _______
    ),
    [_NUM] = LAYOUT_split_70(
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                     KC_PLUS, KC_7,    KC_8,    KC_9,    KC_BSPC, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, _______, KC_LALT, KC_LCTL, KC_LSFT, _______,                                     KC_MINS, KC_4,    KC_5,    KC_6,    KC_ASTR, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_LGUI, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_ENT,  KC_TRNS, KC_1,    KC_2,    KC_3,    KC_DOT,  KC_TRNS, KC_TRNS,
                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_RSFT, KC_0,    KC_TRNS, KC_0,    KC_DOT,  KC_SLSH, KC_TRNS
    ),
    [_WASD1] = LAYOUT_split_70(
                               KC_1,   KC_2,  KC_3,    KC_4,    KC_5,                                           KC_6,    KC_7,     KC_8,     KC_9,     DF(_QWERTY),
            KC_ESC,  KC_TAB,   KC_Q,   KC_W,  KC_E,    KC_R,    KC_T,                                           KC_Y,    KC_U,     KC_I,     KC_O,     KC_0,      KC_MINS,   KC_EQL,
            KC_GRV,  KC_LSFT,  KC_A,   KC_S,  KC_D,    KC_F,    KC_G,                                           KC_H,    KC_J,     KC_K,     KC_L,     KC_P,      KC_LBRC,   KC_RBRC,
            KC_LCTL, KC_LSFT,  KC_Z,   KC_X,  KC_C,    KC_V,    KC_B, KC_ENTER,                         KC_ENTER,  KC_N,    KC_M,     KC_COMM,  KC_DOT,   KC_SCLN,   KC_QUOT,   KC_BSLS,
                     KC_LSFT,  KC_NO,  KC_NO, KC_NO, KC_LCTL, KC_SPC, KC_LALT,   KC_ESC,   KC_TAB, KC_LSFT,    KC_SPC, KC_RCTL,  RMTD,  RMTU,  KC_SLSH,  KC_RSFT

    ),
    [_WASD] = LAYOUT_split_70(
                               KC_1,   KC_2,  KC_3,    KC_4,    KC_5,                                           KC_6,    KC_7,     KC_8,     KC_9,     DF(_QWERTY),
            KC_ESC,  KC_TAB,   KC_Q,   KC_W,  KC_E,    KC_R,    KC_T,                                           KC_Y,    KC_U,     KC_I,     KC_O,     KC_0,      KC_MINS,   KC_EQL,
            KC_GRV,  KC_LSFT,  KC_A,   KC_S,  KC_D,    KC_F,    KC_G,                                           KC_H,    KC_J,     KC_K,     KC_L,     KC_P,      KC_LBRC,   KC_RBRC,
            KC_LCTL, KC_LCTL,  KC_Z,   KC_X,  KC_C,    KC_V,    KC_B, KC_ENTER,                         KC_UP,  KC_N,    KC_M,     KC_COMM,  KC_DOT,   KC_SCLN,   KC_QUOT,   KC_BSLS,
                     KC_LSFT,  KC_NO,  KC_NO, KC_NO, KC_LCTL, KC_SPC, KC_LALT,   KC_ESC,   KC_LEFT, KC_DOWN,   KC_RIGHT, KC_RCTL,  RMTD,  RMTU,  KC_SLSH,  KC_RSFT

    ),
    [_DOUBLE] = LAYOUT_split_70(
                          KC_NO,        KC_NO,       KC_NO, KC_NO, KC_NO,                             KC_NO, QK_USER_3, KC_NO, KC_NO, KC_NO,
            KC_NO, KC_NO, KC_NO,        DF(_WASD),   KC_NO, KC_NO, KC_NO,                             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, QK_USER_4, KC_NO,
            KC_NO, KC_NO, DF(_QWERTY),  DF(_SINGLE), KC_NO, KC_NO, DF(_WASD1),                        KC_NO, KC_NO, KC_NO, KC_NO, QK_USER_2, KC_NO, KC_NO,
            KC_NO, KC_NO, KC_NO,        KC_NO,       QK_USER_0, QK_USER_1, KC_NO, KC_NO,               KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                   KC_NO, KC_NO,        KC_NO,       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO

    ),
    // [_MT_QWERTY] = LAYOUT_split_70(
    //                            KC_ESC,             KC_2,             KC_3,    KC_4,    KC_5,                                                                 KC_6,     KC_7,     KC_8,     KC_9,     KC_DEL,
    //         KC_HOME, KC_GRAVE, KC_1,               VLTW,             VLTE,    KC_R,    KC_T,                                                                 KC_Y,     KC_U,     KC_I,     KC_O,     KC_0,      KC_MINS,   KC_EQL,
    //         KC_END,  KC_TAB,   KC_Q, MT(MOD_LALT, KC_S), MT(MOD_LCTL, KC_D), MT(MOD_LSFT, KC_F),    KC_G,                                KC_H,     MT(MOD_RSFT, KC_J),     MT(MOD_RCTL, KC_K),     MT(MOD_RALT, KC_L),   KC_P,     KC_LBRC,   KC_RBRC,
    //         KC_LCTL, KC_CAPS,  MT(MOD_LGUI, KC_A), KC_X,             KC_C,    KC_V,    KC_B, LT(_NUM, KC_SPC),                                    KC_ENTER,  KC_N,     KC_M,     MT(MOD_RGUI, KC_COMM),  KC_DOT,   KC_SCLN,   KC_QUOT,   KC_BSLS,
    //                  KC_LSFT,  KC_Z,               LMTL,             LMTR, KC_LCTL, KC_LGUI, KC_LALT,   LT(_NAV, KC_BSPC),   LT(_SYM, KC_ESC), KC_RSFT,   KC_RCTL,  KC_RCTL,  RMTD,  RMTU,  KC_SLSH,  KC_RSFT
    //
    // ),
    [_MOUSE] = LAYOUT_split_70(

                              _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, MS_ACL1, MS_ACL2, _______, _______,                                     MS_LEFT, MS_DOWN, MS_UP,   MS_RGHT, _______, _______, _______,
            _______, _______, MS_ACL0, _______, _______, _______, _______, _______,                   _______, _______, MS_BTN1, MS_BTN2, _______, _______, _______, _______,
                     _______, _______, _______, _______, MS_BTN1, _______, _______, _______, _______, _______, _______, MS_BTN2, _______, _______, _______, _______

    ),
};

// clang-format on

void tap_8_times_or_once_delay(uint16_t keycode, uint16_t delay) {
    if (is_rep_8_active) {
        tap_code16_delay(keycode, delay);
        tap_code16_delay(keycode, delay);
        tap_code16_delay(keycode, delay);
        tap_code16_delay(keycode, delay);
        tap_code16_delay(keycode, delay);
        tap_code16_delay(keycode, delay);
        tap_code16_delay(keycode, delay);
        tap_code16_delay(keycode, delay);
    } else {
        tap_code16_delay(keycode, delay);
    }
}

void tap_8_times_or_once(uint16_t keycode) {
    if (is_rep_8_active) {
        tap_code16(keycode);
        tap_code16(keycode);
        tap_code16(keycode);
        tap_code16(keycode);
        tap_code16(keycode);
        tap_code16(keycode);
        tap_code16(keycode);
        tap_code16(keycode);
    } else {
        tap_code16(keycode);
    }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    ACTIVATE_LAYER_KEY(is_8_held, is_rep_8_active, is_rep_8_timer);
    ACTIVATE_LAYER_KEY(is_i_held, is_rep_8_active, is_rep_8_timer);
    ACTIVATE_LAYER_KEY(is_pgup_held, is_rep_8_active, is_rep_8_timer);
    ACTIVATE_LAYER_KEY(is_g_held, is_vim_g_active, is_vim_g_timer);
    ACTIVATE_LAYER_KEY(is_w_held, is_vim_wb_active, is_vim_wb_timer);
    ACTIVATE_LAYER_KEY(is_e_held, is_vim_ege_active, is_vim_ege_timer);
    ACTIVATE_LAYER_KEY(is_tab_held, is_sc_tab_active, is_sc_tab_timer);
    ACTIVATE_LAYER_KEY(is_u_held, is_strange_active, is_strange_timer);
    ACTIVATE_LAYER_KEY(is_y_held, is_redo_y_active, is_redo_y_timer);
    ACTIVATE_LAYER_KEY(is_z_held, is_undo_z_active, is_undo_z_timer);

    if (index == 0) { /* First encoder */
        if (!clockwise) {
            if (is_strange_active) {
                tap_8_times_or_once(KC_U);
            } else if (is_vim_wb_active) {
                tap_8_times_or_once(KC_B);
            } else if (is_vim_ege_active) {
                if (is_rep_8_active) {
                    tap_code(KC_G);
                    tap_code(KC_E);
                    tap_code(KC_G);
                    tap_code(KC_E);
                    tap_code(KC_G);
                    tap_code(KC_E);
                    tap_code(KC_G);
                    tap_code(KC_E);
                    tap_code(KC_G);
                    tap_code(KC_E);
                    tap_code(KC_G);
                    tap_code(KC_E);
                    tap_code(KC_G);
                    tap_code(KC_E);
                    tap_code(KC_G);
                    tap_code(KC_E);
                } else {
                    tap_code(KC_G);
                    tap_code(KC_E);
                }
            } else if (is_sc_tab_active) {
                tap_8_times_or_once(S(KC_TAB));
            } else if (is_redo_y_active) {
                tap_8_times_or_once(C(KC_Z));
            } else if (is_undo_z_active) {
                tap_8_times_or_once(C(KC_Z));
            } else {
                switch (get_highest_layer(layer_state)) {
                    case _SYM:
                        tap_8_times_or_once(KC_BSPC);
                        break;
                    case _NAV:
                        tap_8_times_or_once(KC_VOLD);
                        break;
                    case _MOUSE:
                        tap_8_times_or_once_delay(MS_LEFT, SCROLL_MOUSE_DELAY);
                        break;
                    default:
                        tap_8_times_or_once(KC_LEFT);
                        break;
                }
            }
        } else {
            if (is_strange_active) {
                tap_8_times_or_once(C(KC_R));
            } else if (is_vim_wb_active) {
                tap_8_times_or_once(KC_W);
            } else if (is_vim_ege_active) {
                tap_8_times_or_once(KC_E);
            } else if (is_sc_tab_active) {
                tap_8_times_or_once(KC_TAB);
            } else if (is_redo_y_active) {
                tap_8_times_or_once(C(KC_Y));
            } else if (is_undo_z_active) {
                tap_8_times_or_once(S(C(KC_Z)));
            } else {
                switch (get_highest_layer(layer_state)) {
                    case _SYM:
                        tap_8_times_or_once(KC_DEL);
                        break;
                    case _NAV:
                        tap_8_times_or_once(KC_VOLU);
                        break;
                    case _MOUSE:
                        tap_8_times_or_once_delay(MS_RGHT, SCROLL_MOUSE_DELAY);
                        break;
                    default:
                        tap_8_times_or_once(KC_RIGHT);
                        break;
                }
            }
        }
    } else if (index == 1) { /* Second encoder */
        if (!clockwise) {
            if (is_strange_active) {
                tap_8_times_or_once(C(KC_R));
            } else if (is_vim_wb_active) {
                tap_8_times_or_once_delay(KC_RCBR, 10);
            } else if (is_vim_ege_active) {
                tap_8_times_or_once(KC_DOWN);
            } else if (is_sc_tab_active) {
                tap_8_times_or_once(S(KC_TAB));
            } else if (is_redo_y_active) {
                tap_8_times_or_once(C(KC_Z));
            } else if (is_undo_z_active) {
                tap_8_times_or_once(C(KC_Z));
            } else if (is_vim_g_active) {
                if (is_rep_8_active) {
                    tap_code(KC_G);
                    tap_code(KC_DOWN);
                    tap_code(KC_G);
                    tap_code(KC_DOWN);
                    tap_code(KC_G);
                    tap_code(KC_DOWN);
                    tap_code(KC_G);
                    tap_code(KC_DOWN);
                    tap_code(KC_G);
                    tap_code(KC_DOWN);
                    tap_code(KC_G);
                    tap_code(KC_DOWN);
                    tap_code(KC_G);
                    tap_code(KC_DOWN);
                    tap_code(KC_G);
                    tap_code(KC_DOWN);
                } else {
                    tap_code(KC_G);
                    tap_code(KC_DOWN);
                }

            } else {
                switch (get_highest_layer(layer_state)) {
                    case _SYM:
                        tap_8_times_or_once(KC_BSPC);
                        break;
                    case _NAV:
                        tap_8_times_or_once(KC_MS_WH_DOWN);
                        break;
                    case _MOUSE:
                        tap_8_times_or_once_delay(MS_DOWN, SCROLL_MOUSE_DELAY);
                        break;
                    case _NUM:
                        register_mods(get_last_mods());
                        tap_8_times_or_once(get_alt_repeat_key_keycode());
                        unregister_mods(get_last_mods());
                        break;
                    default:
                        tap_8_times_or_once(KC_DOWN);
                        break;
                }
            }

        } else {
            if (is_strange_active) {
                tap_8_times_or_once(KC_U);
            } else if (is_vim_wb_active) {
                tap_8_times_or_once(S(KC_LBRC));
            } else if (is_vim_ege_active) {
                tap_8_times_or_once(KC_UP);
            } else if (is_sc_tab_active) {
                tap_8_times_or_once(KC_TAB);
            } else if (is_redo_y_active) {
                tap_8_times_or_once(C(KC_Y));
            } else if (is_undo_z_active) {
                tap_8_times_or_once(C(S(KC_Z)));
            } else if (is_vim_g_active) {
                if (is_rep_8_active) {
                    tap_code(KC_G);
                    tap_code(KC_UP);
                    tap_code(KC_G);
                    tap_code(KC_UP);
                    tap_code(KC_G);
                    tap_code(KC_UP);
                    tap_code(KC_G);
                    tap_code(KC_UP);
                    tap_code(KC_G);
                    tap_code(KC_UP);
                    tap_code(KC_G);
                    tap_code(KC_UP);
                    tap_code(KC_G);
                    tap_code(KC_UP);
                    tap_code(KC_G);
                    tap_code(KC_UP);
                } else {
                    tap_code(KC_G);
                    tap_code(KC_UP);
                }
            } else {
                switch (get_highest_layer(layer_state)) {
                    case _SYM:
                        tap_8_times_or_once(KC_DEL);
                        break;
                    case _NAV:
                        tap_8_times_or_once(KC_MS_WH_UP);
                        break;
                    case _MOUSE:
                        tap_8_times_or_once_delay(MS_UP, SCROLL_MOUSE_DELAY);
                        break;
                    case _NUM:
                        register_mods(get_last_mods());
                        tap_8_times_or_once(get_last_keycode());
                        unregister_mods(get_last_mods());
                        break;
                    default:
                        tap_8_times_or_once(KC_UP);
                        break;
                }
            }
        }
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (is_scroll_layer_deactivated) {
        switch (keycode) {
            case (uint16_t)DF(_QWERTY):
                break;
            default:
                return true;
        }
    }
    switch (keycode) {
            PROCESS_SCROLL_LAYER(KC_8, is_8_held, is_rep_8_active, is_rep_8_timer, key_8_mods);
            PROCESS_SCROLL_LAYER(KC_PGUP, is_pgup_held, is_rep_8_active, is_rep_8_timer, key_pgup_mods);
            PROCESS_SCROLL_LAYER(KC_I, is_i_held, is_rep_8_active, is_rep_8_timer, key_i_mods);
            PROCESS_SCROLL_LAYER(KC_G, is_g_held, is_vim_g_active, is_vim_g_timer, key_g_mods);
            PROCESS_SCROLL_LAYER(KC_W, is_w_held, is_vim_wb_active, is_vim_wb_timer, key_w_mods);
            PROCESS_SCROLL_LAYER(KC_E, is_e_held, is_vim_ege_active, is_vim_ege_timer, key_e_mods);
            PROCESS_SCROLL_LAYER(KC_TAB, is_tab_held, is_sc_tab_active, is_sc_tab_timer, key_tab_mods);
            PROCESS_SCROLL_LAYER(KC_U, is_u_held, is_strange_active, is_strange_timer, key_u_mods);
            PROCESS_SCROLL_LAYER(KC_Y, is_y_held, is_redo_y_active, is_redo_y_timer, key_y_mods)
            // PROCESS_SCROLL_LAYER(KC_Z, is_z_held, is_undo_z_active, is_undo_z_timer)

        case KC_Z:
            if (record->event.pressed) {
                is_z_held       = 1;
                is_undo_z_timer = timer_read();
            } else {
                if (is_z_held) {
                    tap_code(KC_Z);
                }
                is_z_held        = 0;
                is_undo_z_active = 0;
                is_undo_z_timer  = timer_read();
            }
            return 0;
        case (uint16_t)DF(_QWERTY):
            if (!record->event.pressed) {
                is_scroll_layer_deactivated = false;
            }
            break;
        case (uint16_t)DF(_WASD):
        case (uint16_t)DF(_WASD1):

            if (!record->event.pressed) {
                is_scroll_layer_deactivated = true;

                is_scroll_layer_deactivated = true;
                is_rep_8_active             = false;
                is_vim_wb_active            = false;
                is_vim_ege_active           = false;
                is_sc_tab_active            = false;
                is_strange_active           = false;
                is_redo_y_active            = false;
                is_undo_z_active            = false;
                is_vim_g_active             = false;

                is_8_held    = false;
                is_i_held    = false;
                is_pgup_held = false;
                is_w_held    = false;
                is_e_held    = false;
                is_tab_held  = false;
                is_u_held    = false;
                is_y_held    = false;
                is_z_held    = false;
                is_g_held    = false;
            }
            break;

        default:
            CHECK_AND_TAP(KC_8, is_8_held, is_rep_8_active, is_rep_8_timer, keycode);
            CHECK_AND_TAP(KC_I, is_i_held, is_rep_8_active, is_rep_8_timer, keycode);
            CHECK_AND_TAP(KC_PGUP, is_pgup_held, is_rep_8_active, is_rep_8_timer, keycode);
            CHECK_AND_TAP(KC_W, is_w_held, is_vim_wb_active, is_vim_wb_timer, keycode);
            CHECK_AND_TAP(KC_G, is_g_held, is_vim_g_active, is_vim_g_timer, keycode);
            CHECK_AND_TAP(KC_E, is_e_held, is_vim_ege_active, is_vim_ege_timer, keycode);
            CHECK_AND_TAP(KC_TAB, is_tab_held, is_sc_tab_active, is_sc_tab_timer, keycode);
            CHECK_AND_TAP(KC_U, is_u_held, is_strange_active, is_strange_timer, keycode);
            CHECK_AND_TAP(KC_Y, is_y_held, is_redo_y_active, is_redo_y_timer, keycode);
            CHECK_AND_TAP(KC_Z, is_z_held, is_undo_z_active, is_undo_z_timer, keycode);
            return true; // Process all other keycodes normally
    }
    return true;
}

void matrix_scan_user(void) {
    if (is_scroll_layer_deactivated) {
        return;
    }

    TRY_TO_ACTIVATE_LAYER_KEY(is_8_held, is_rep_8_timer, is_rep_8_active, CUSTOM_TAP_LAYER_TIMEOUT);
    TRY_TO_ACTIVATE_LAYER_KEY(is_i_held, is_rep_8_timer, is_rep_8_active, CUSTOM_TAP_LAYER_TIMEOUT);
    TRY_TO_ACTIVATE_LAYER_KEY(is_pgup_held, is_rep_8_timer, is_rep_8_active, CUSTOM_TAP_LAYER_TIMEOUT);
    TRY_TO_ACTIVATE_LAYER_KEY(is_w_held, is_vim_wb_timer, is_vim_wb_active, CUSTOM_TAP_LAYER_TIMEOUT);
    TRY_TO_ACTIVATE_LAYER_KEY(is_g_held, is_vim_g_timer, is_vim_g_active, CUSTOM_TAP_LAYER_TIMEOUT);
    TRY_TO_ACTIVATE_LAYER_KEY(is_e_held, is_vim_ege_timer, is_vim_ege_active, CUSTOM_TAP_LAYER_TIMEOUT);
    TRY_TO_ACTIVATE_LAYER_KEY(is_tab_held, is_sc_tab_timer, is_sc_tab_active, CUSTOM_TAP_LAYER_TIMEOUT);
    TRY_TO_ACTIVATE_LAYER_KEY(is_u_held, is_strange_timer, is_strange_active, CUSTOM_TAP_LAYER_TIMEOUT);
    TRY_TO_ACTIVATE_LAYER_KEY(is_y_held, is_redo_y_timer, is_redo_y_active, CUSTOM_TAP_LAYER_TIMEOUT);
    TRY_TO_ACTIVATE_LAYER_KEY(is_z_held, is_undo_z_timer, is_undo_z_active, CUSTOM_TAP_LAYER_TIMEOUT);
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(_SYM, KC_BSPC):
            return DELETE_TAPPING_TERM;
        default:
            return TAPPING_TERM;
    }
}

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    if ((mods & MOD_MASK_CTRL)) {
        switch (keycode) {
            case KC_O:
                return C(KC_I); // Vim jump list
            case KC_I:
                return C(KC_O);
            case KC_A:
                return C(KC_X); // Vim increase/decrease number
            case KC_X:
                return C(KC_A);
        }
    } else if ((mods & MOD_MASK_SHIFT)) {
        switch (keycode) {
            case KC_N:
                return KC_N; // Vim search
        }
    } else if (mods == 0x00) {
        switch (keycode) {
            case KC_SCLN:
                return KC_COMM;
            case KC_COMM:
                return KC_SCLN;
            case KC_N:
                return S(KC_N);
        }
    }

    return KC_TRNS; // Defer to default definitions.
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(_NUM, KC_SPC):
        case LT(_MOUSE, KC_V):
            return false;
        default:
            return true;
    }
}

// layer_state_t layer_state_set_user(layer_state_t state) {
// // This does not work
//     switch (get_highest_layer(state)) {
//         case _WASD:
//             is_scroll_layer_deactivated = true;
//             is_rep_8_active = false;
//             is_vim_wb_active = false;
//             is_vim_ege_active = false;
//             is_sc_tab_active = false;
//             is_strange_active = false;
//             is_redo_y_active = false;
//             is_undo_z_active = false;
//             is_vim_g_active = false;
//
//
//             is_8_held = false;
//             is_i_held = false;
//             is_pgup_held = false;
//             is_w_held = false;
//             is_e_held = false;
//             is_tab_held = false;
//             is_u_held = false;
//             is_y_held = false;
//             is_z_held = false;
//             is_g_held = false;
//         break;
//         case _QWERTY:
//             is_scroll_layer_deactivated = false;
//         break;
//     }
//   return state;
// }
