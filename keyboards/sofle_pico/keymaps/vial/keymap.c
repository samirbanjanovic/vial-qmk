// Copyright 2024 Ryan Neff (@JellyTitan)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

static uint16_t session_word_count = 0;

enum custom_keycodes {
    RAINBOW_RAIN = QK_KB_0,
    RAINBOW_POOLS,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == RAINBOW_RAIN || keycode == RAINBOW_POOLS) {
        if (record->event.pressed) {
            rgb_matrix_enable();
            rgb_matrix_mode(keycode == RAINBOW_RAIN ? RGB_MATRIX_CUSTOM_RAINBOW_RAIN : RGB_MATRIX_CUSTOM_RAINBOW_POOLS);
        }
        return false;
    }
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

// Defaults imported from default-sofle-layout.vil.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // clang-format off
    [_QWERTY] = LAYOUT(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_HOME,
        KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L,    KC_QUOT, KC_END,
        KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    TL_LOWR, KC_RBRC,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SCLN, KC_DEL,
                         KC_ESC, KC_LGUI, KC_LALT, KC_SPC, TL_LOWR,    KC_ENT, KC_BSPC, KC_SLSH, KC_BSLS, KC_EQL
    ),
    [_LOWER] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                         KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,       KC_F12,
        KC_NO,   KC_NO,   KC_MUTE, KC_VOLD, KC_VOLU, KC_NO,                         KC_NO,   KC_NO,   KC_UP,   KC_NO,   KC_NO,        RGB_TOG,
        KC_CAPS, KC_NO,   KC_MPRV, KC_MPLY, KC_MNXT, KC_NO,                         KC_NO,   KC_LEFT, KC_DOWN, KC_RGHT, RAINBOW_RAIN, RGB_MOD,
        KC_LCTL, KC_LSFT, KC_NO,   KC_NO,   KC_NO,   KC_NO,   TL_LOWR, KC_NO,       KC_NO,   KC_NO,   KC_NO,   KC_NO,   RAINBOW_POOLS, RGB_RMOD,
                         KC_ESC, KC_NO, KC_NO, KC_NO, TL_LOWR,    KC_ENT, KC_BSPC, KC_NO, KC_NO, KC_NO
    ),
    [_RAISE] = LAYOUT(
        _______, _______, _______, _______, _______, _______,                         _______, _______, _______, _______, _______, _______,
        _______, KC_INS,  KC_PSCR, KC_APP,  KC_NO,   KC_NO,                           KC_PGUP, KC_NO,   KC_UP,   KC_NO,   KC_NO,   KC_BSPC,
        _______, KC_LALT, KC_LCTL, KC_LSFT, KC_NO,   KC_CAPS,                         KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL,  KC_BSPC,
        _______, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), KC_NO, _______,       _______,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   _______,
                          _______, _______, _______, _______, _______,        _______, _______, _______, _______, _______
    ),
    [_ADJUST] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                         KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,
        QK_BOOT, KC_NO,   KC_NO,   KC_NO,   CG_TOGG, KC_NO,                         KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,
        KC_NO,   KC_NO,   CG_TOGG, KC_NO,   KC_NO,   KC_NO,                         KC_NO, KC_VOLD, KC_MUTE, KC_VOLU, KC_NO, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,       KC_NO,     KC_NO, KC_MPRV, KC_MPLY, KC_MNXT, KC_NO, KC_NO,
                          _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
    )
    // clang-format on
};

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_90;
}

#ifdef RGB_MATRIX_ENABLE
static const char *rgb_matrix_mode_name(void) {
    switch (rgb_matrix_get_mode()) {
#define RGB_MATRIX_EFFECT(name, ...) case RGB_MATRIX_##name: return PSTR(#name);
#include "rgb_matrix_effects.inc"
#undef RGB_MATRIX_EFFECT
#define RGB_MATRIX_EFFECT(name, ...) case RGB_MATRIX_CUSTOM_##name: return PSTR(#name);
#include "rgb_matrix_user.inc"
#undef RGB_MATRIX_EFFECT
        default: return PSTR("UNKNOWN");
    }
}

static void render_rgb_mode(void) {
    const char *name = rgb_matrix_is_enabled() ? rgb_matrix_mode_name() : PSTR("OFF");
    char text[31] = "RGB: ";
    uint8_t index = 5;

    while (index < sizeof(text) - 1) {
        char c = pgm_read_byte(name++);
        if (c == '\0') {
            break;
        }
        text[index++] = c == '_' ? ' ' : c;
    }
    while (index < sizeof(text) - 1) {
        text[index++] = ' ';
    }
    text[index] = '\0';

    for (uint8_t row = 0; row < 3; row++) {
        oled_set_cursor(0, 9 + row);
        for (uint8_t column = 0; column < 10; column++) {
            oled_write_char(text[row * 10 + column], false);
        }
    }
}
#endif

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

    // Row 7: Live Speed
    oled_set_cursor(0, 7);
    #ifdef WPM_ENABLE
    char wpm_buf[16];
    snprintf(wpm_buf, sizeof(wpm_buf), "WPM:   %03d", get_current_wpm());
    oled_write(wpm_buf, false);
    #else
    oled_write_P(PSTR("WPM:   ---"), false);
    #endif

    // Row 8: Session Word Count
    oled_set_cursor(0, 8);
    char word_buf[16];
    snprintf(word_buf, sizeof(word_buf), "WORDS:%04d", session_word_count);
    oled_write(word_buf, false);

    #ifdef RGB_MATRIX_ENABLE
    render_rgb_mode();
    #endif

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