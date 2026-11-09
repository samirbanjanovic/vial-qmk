// SPDX-License-Identifier: GPL-2.0-or-later

RGB_MATRIX_EFFECT(RAINBOW_RAIN)
RGB_MATRIX_EFFECT(RAINBOW_POOLS)

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

typedef struct {
    uint32_t started;
    uint16_t duration;
    uint16_t delay;
    uint8_t led;
    uint8_t radius;
    uint8_t level;
    RGB color;
} rainbow_pool_t;

static bool RAINBOW_POOLS(effect_params_t *params) {
    static rainbow_pool_t pools[2][3];
    static uint32_t animation_time;
    static uint32_t last_frame;
    static uint8_t time_remainder;
    const uint8_t split[] = RGB_MATRIX_SPLIT;

    if (params->iter == 0) {
        if (params->init) {
            memset(pools, 0, sizeof(pools));
            animation_time = 0;
            last_frame = g_rgb_timer;
            time_remainder = 0;
            rgb_matrix_set_color_all(0, 0, 0);
        }

        const uint64_t elapsed = (uint64_t)(uint32_t)(g_rgb_timer - last_frame) * (rgb_matrix_config.speed + 64u) + time_remainder;
        animation_time += elapsed / 128;
        time_remainder = elapsed % 128;
        last_frame = g_rgb_timer;

        for (uint8_t half = 0; half < 2; half++) {
            for (uint8_t index = 0; index < 3; index++) {
                rainbow_pool_t *pool = &pools[half][index];
                uint32_t age = animation_time - pool->started;
                if (age >= (uint32_t)pool->duration + pool->delay) {
                    pool->started = animation_time;
                    pool->duration = 1800 + (uint16_t)random8() * 8;
                    pool->delay = (uint16_t)random8() * 4;
                    pool->led = (half ? split[0] : 0) + random8_max(split[half]);
                    pool->radius = 32 + random8_max(25);
                    pool->color = rgb_matrix_hsv_to_rgb((HSV){random8(), UINT8_MAX, UINT8_MAX});
                    age = 0;
                }
                if (age < pool->delay) {
                    pool->level = 0;
                } else {
                    const uint8_t phase = (age - pool->delay) * 256 / pool->duration;
                    pool->level = sin8((uint8_t)(phase - 64));
                }
            }
        }
    }

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        const uint8_t half = i < split[0] ? 0 : 1;
        uint16_t red = 0, green = 0, blue = 0;
        for (uint8_t index = 0; index < 3; index++) {
            const rainbow_pool_t *pool = &pools[half][index];
            const int16_t dx = (int16_t)g_led_config.point[i].x - g_led_config.point[pool->led].x;
            const int16_t dy = (int16_t)g_led_config.point[i].y - g_led_config.point[pool->led].y;
            const uint8_t distance = sqrt16(dx * dx + dy * dy);
            if (distance >= pool->radius) {
                continue;
            }
            const uint8_t falloff = (uint16_t)(pool->radius - distance) * 255 / pool->radius;
            const uint8_t intensity = scale8(falloff, pool->level);
            red += scale8(pool->color.r, intensity);
            green += scale8(pool->color.g, intensity);
            blue += scale8(pool->color.b, intensity);
        }
        // Blend overlapping pools without exceeding the selected brightness.
        const uint16_t divisor = MAX(255, MAX(red, MAX(green, blue)));
        rgb_matrix_set_color(i,
                             (uint32_t)red * rgb_matrix_config.hsv.v / divisor,
                             (uint32_t)green * rgb_matrix_config.hsv.v / divisor,
                             (uint32_t)blue * rgb_matrix_config.hsv.v / divisor);
    }
    return rgb_matrix_check_finished_leds(led_max);
}
#endif
