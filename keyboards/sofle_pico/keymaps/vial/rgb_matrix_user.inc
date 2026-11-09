// SPDX-License-Identifier: GPL-2.0-or-later

RGB_MATRIX_EFFECT(RAINBOW_RAIN)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool RAINBOW_RAIN(effect_params_t *params) {
    static uint8_t brightness[MATRIX_ROWS][MATRIX_COLS];
    static uint8_t heads[2][MATRIX_COLS];
    static uint32_t last_step;
    const uint8_t rows_per_half = MATRIX_ROWS / 2;
    const uint16_t interval = 280 - rgb_matrix_config.speed;

    if (params->iter == 0) {
        if (params->init) {
            memset(brightness, 0, sizeof(brightness));
            memset(heads, UINT8_MAX, sizeof(heads));
            last_step = g_rgb_timer - interval;
            rgb_matrix_set_color_all(0, 0, 0);
        }

        if ((uint32_t)(g_rgb_timer - last_step) >= interval) {
            last_step = g_rgb_timer;
            for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
                for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                    brightness[row][col] = qsub8(brightness[row][col], 64);
                }
            }

            for (uint8_t half = 0; half < 2; half++) {
                for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                    uint8_t *head = &heads[half][col];
                    if (*head < rows_per_half) {
                        (*head)++;
                    } else if (random8_max(4) == 0) {
                        *head = 0;
                    }
                    if (*head < rows_per_half) {
                        brightness[half * rows_per_half + *head][col] = UINT8_MAX;
                    }
                }
            }
        }
    }

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        // Split matrix rows are stacked; both halves use the same physical row hues.
        const uint8_t hue = (uint16_t)(row % rows_per_half) * 256 / rows_per_half;
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            const uint8_t led = g_led_config.matrix_co[row][col];
            if (led < led_min || led >= led_max || !HAS_ANY_FLAGS(g_led_config.flags[led], params->flags)) {
                continue;
            }
            const HSV hsv = {hue, UINT8_MAX, scale8(brightness[row][col], rgb_matrix_config.hsv.v)};
            const RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
            rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}
#endif
