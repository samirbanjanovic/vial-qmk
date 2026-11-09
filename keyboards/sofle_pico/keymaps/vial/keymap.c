// Copyright 2024 Ryan Neff (@JellyTitan)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

static uint16_t session_word_count = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (keycode == KC_SPC || keycode == KC_ENT) {
            session_word_count++;
        }
    }
    return true;
}

enum sofle_layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // clang-format off
    [_QWERTY] = LAYOUT(
        KC_GRV,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                          KC_6,    KC_7,    KC_8,    KC_9,   KC_0,    KC_GRV,
        KC_ESC,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                          KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,    KC_BSPC,
        KC_TAB,   KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                          KC_H,    KC_J,    KC_K,    KC_L,   KC_SCLN, KC_QUOT,
        KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_MUTE,     XXXXXXX, KC_N,    KC_M,    KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
                          KC_LGUI, KC_LALT, KC_LCTL, TL_LOWR, KC_ENT,      KC_SPC , TL_UPPR, KC_RCTL, KC_RALT, KC_RGUI
    ),
    [_LOWER] = LAYOUT(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                           KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                            KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_F12,
        _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                         KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
        _______, KC_EQL,  KC_MINS, KC_PLUS, KC_LCBR, KC_RCBR, _______,       _______, KC_LBRC, KC_RBRC, KC_SCLN, KC_COLN, KC_BSLS, _______,
                          _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
    ),
    [_RAISE] = LAYOUT(
        _______, _______, _______, _______, _______, _______,                          _______, _______, _______, _______, _______, _______,
        _______, KC_INS,  KC_PSCR, KC_APP,  XXXXXXX, XXXXXXX,                          KC_PGUP, XXXXXXX, KC_UP  , XXXXXXX, XXXXXXX, KC_BSPC,
        _______, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, KC_CAPS,                          KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL , KC_BSPC,
        _______, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), XXXXXXX, _______,        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                          _______, _______, _______, _______, _______,        _______, _______, _______, _______, _______
    ),
    [_ADJUST] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, CG_TOGG, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, CG_TOGG, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
                          _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
    )
    // clang-format on
};

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_90;
}

// Master Display: Spanning rows 0 through 14
static void render_master_display(void) {
    // Row 0: Top Header
    oled_set_cursor(0, 0);
    oled_write_P(PSTR("==STATUS=="), false);

    // Row 1: Active Layer
    oled_set_cursor(0, 1);
    oled_write_P(PSTR("LYR: "), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY: oled_write_P(PSTR("BASE "), false); break;
        case _LOWER:  oled_write_P(PSTR("LOWR "), false); break;
        case _RAISE:  oled_write_P(PSTR("RAIS "), false); break;
        case _ADJUST: oled_write_P(PSTR("ADJ  "), false); break;
        default:      oled_write_P(PSTR("USER "), false); break;
    }

    // Row 2: Divider
    oled_set_cursor(0, 2);
    oled_write_P(PSTR("----------"), false);

    // Row 3-4: 2x2 Modifier Grid
    uint8_t mods = get_mods();
    oled_set_cursor(0, 3);
    oled_write_P((mods & MOD_MASK_GUI) ? PSTR("[GUI]") : PSTR("[   ]"), false);
    oled_write_P((mods & MOD_MASK_ALT) ? PSTR("[ALT]") : PSTR("[   ]"), false);

    oled_set_cursor(0, 4);
    oled_write_P((mods & MOD_MASK_CTRL)  ? PSTR("[CTL]") : PSTR("[   ]"), false);
    oled_write_P((mods & MOD_MASK_SHIFT) ? PSTR("[SFT]") : PSTR("[   ]"), false);

    // Row 5: Divider
    oled_set_cursor(0, 5);
    oled_write_P(PSTR("----------"), false);

    // Row 6: Caps Lock
    oled_set_cursor(0, 6);
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.caps_lock ? PSTR("CAPS: [ON]") : PSTR("CAPS:  OFF"), false);

    // Row 7: Blank Spacer
    oled_set_cursor(0, 7);
    oled_write_P(PSTR("          "), false);

    // Row 8: Live Speed
    oled_set_cursor(0, 8);
    #ifdef WPM_ENABLE
    char wpm_buf[16];
    snprintf(wpm_buf, sizeof(wpm_buf), "WPM:   %03d", get_current_wpm());
    oled_write(wpm_buf, false);
    #else
    oled_write_P(PSTR("WPM:   ---"), false);
    #endif

    // Row 9: Blank Spacer
    oled_set_cursor(0, 9);
    oled_write_P(PSTR("          "), false);

    // Row 10: Session Word Count
    oled_set_cursor(0, 10);
    char word_buf[16];
    snprintf(word_buf, sizeof(word_buf), "WORDS:%04d", session_word_count);
    oled_write(word_buf, false);

    // Row 11: Blank Spacer
    oled_set_cursor(0, 11);
    oled_write_P(PSTR("          "), false);

    // Row 12: Dynamic Hardware Handedness
    oled_set_cursor(0, 12);
    oled_write_P(is_keyboard_left() ? PSTR("SIDE: LEFT") : PSTR("SIDE: RGHT"), false);

    // Row 13: Divider
    oled_set_cursor(0, 13);
    oled_write_P(PSTR("----------"), false);

    // Row 14: Bottom Border
    oled_set_cursor(0, 14);
    oled_write_P(PSTR("=========="), false);
}

// Slave Display: Static Hardware Telemetry
static void render_slave_display(void) {
    // Row 0: Static Header
    oled_set_cursor(0, 0);
    oled_write_P(PSTR("=HARDWARE="), false);

    // Row 1: Layer Mirror
    oled_set_cursor(0, 1);
    oled_write_P(PSTR("LYR: "), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY: oled_write_P(PSTR("BASE "), false); break;
        case _LOWER:  oled_write_P(PSTR("LOWR "), false); break;
        case _RAISE:  oled_write_P(PSTR("RAIS "), false); break;
        case _ADJUST: oled_write_P(PSTR("ADJ  "), false); break;
        default:      oled_write_P(PSTR("USER "), false); break;
    }

    // Row 2: Divider
    oled_set_cursor(0, 2);
    oled_write_P(PSTR("----------"), false);

    // Row 3: Dynamic Physical Side (LEFT or RGHT)
    oled_set_cursor(0, 3);
    oled_write_P(is_keyboard_left() ? PSTR("SIDE: LEFT") : PSTR("SIDE: RGHT"), false);

    // Row 4: Dynamic Split Role (MSTR or SLAV)
    oled_set_cursor(0, 4);
    oled_write_P(is_keyboard_master() ? PSTR("ROLE: MSTR") : PSTR("ROLE: SLAV"), false);

    // Row 5: Physical Interconnect Type
    oled_set_cursor(0, 5);
    oled_write_P(PSTR("LINK: USBC"), false);

    // Row 6: Divider
    oled_set_cursor(0, 6);
    oled_write_P(PSTR("----------"), false);

    // Row 7: Blank Spacer
    oled_set_cursor(0, 7);
    oled_write_P(PSTR("          "), false);

    // Row 8: Controller Architecture
    oled_set_cursor(0, 8);
    oled_write_P(PSTR("MCU:RP2040"), false);

    // Row 9: Matrix Layout
    oled_set_cursor(0, 9);
    oled_write_P(PSTR("GRID: 5x12"), false);

    // Row 10: Firmware Target
    oled_set_cursor(0, 10);
    oled_write_P(PSTR("FW:   VIAL"), false);

    // Row 11: Blank Spacer
    oled_set_cursor(0, 11);
    oled_write_P(PSTR("          "), false);

    // Row 12: Generic USB Interface Class
    oled_set_cursor(0, 12);
    oled_write_P(PSTR("USB:   HID"), false);

    // Row 13: Divider
    oled_set_cursor(0, 13);
    oled_write_P(PSTR("----------"), false);

    // Row 14: Bottom Border
    oled_set_cursor(0, 14);
    oled_write_P(PSTR("=========="), false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        render_master_display();
    } else {
        render_slave_display();
    }
    return false;
}

#endif // OLED_ENABLE