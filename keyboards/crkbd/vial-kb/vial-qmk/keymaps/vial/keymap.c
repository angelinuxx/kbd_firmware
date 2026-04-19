#include QMK_KEYBOARD_H

#ifdef OLED_ENABLE
#include "frames.inc"

static uint8_t  current_frame = 0;
static uint32_t anim_timer    = 0;

#pragma GCC push_options
#pragma GCC optimize("O2")
static void render_tetris(void) {
    if (timer_elapsed32(anim_timer) > anim_frame_duration) {
        anim_timer    = timer_read32();
        current_frame = (current_frame + 1) % frame_count;
    }
    oled_write_raw_P(tetris_frames[current_frame], frame_size);
}
#pragma GCC pop_options

static void render_draw(void) {
   render_tetris();
}

static void render_layer(uint8_t line) {
    oled_set_cursor(0, line);
    oled_write_P(PSTR("L: "), false);
    switch (get_highest_layer(layer_state)) {
        case 0: oled_write_P(PSTR("DEFAULT"), false); break;
        case 1: oled_write_P(PSTR("LOWER "), false); break;
        case 2: oled_write_P(PSTR("RAISE "), false); break;
        case 3: oled_write_P(PSTR("ADJUST"), false); break;
        default: oled_write_P(PSTR("?    "), false); break;
    }
}

static void render_modifier_state(uint8_t line) {
    oled_set_cursor(0,line);
    if (get_mods() & MOD_MASK_GUI) {
        oled_write_char(137,false);
        oled_write_char(138,false);
        oled_set_cursor(0,line+1);
        oled_write_char(169,false);
        oled_write_char(170,false);
    } else {
        oled_write_char(129,false);
        oled_write_char(130,false);
        oled_set_cursor(0,line+1);
        oled_write_char(161,false);
        oled_write_char(162,false);
    }
    oled_set_cursor(2,line);
    if (get_mods() & MOD_MASK_ALT) {
        oled_write_char(139,false);
        oled_write_char(140,false);
        oled_set_cursor(2,line+1);
        oled_write_char(171,false);
        oled_write_char(172,false);
    } else {
        oled_write_char(131,false);
        oled_write_char(132,false);
        oled_set_cursor(2,line+1);
        oled_write_char(163,false);
        oled_write_char(164,false);
    }
    oled_set_cursor(4,line);
    if (get_mods() & MOD_MASK_CTRL) {
        oled_write_char(143,false);
        oled_write_char(144,false);
        oled_set_cursor(4,line+1);
        oled_write_char(175,false);
        oled_write_char(176,false);
    } else {
        oled_write_char(135,false);
        oled_write_char(136,false);
        oled_set_cursor(4,line+1);
        oled_write_char(167,false);
        oled_write_char(168,false);
    }
    oled_set_cursor(6,line);
    if ((get_mods() & MOD_MASK_SHIFT)) {
        oled_write_char(141,false);
        oled_write_char(142,false);
        oled_set_cursor(6,line+1);
        oled_write_char(173,false);
        oled_write_char(174,false);
    } else {
        oled_write_char(133,false);
        oled_write_char(134,false);
        oled_set_cursor(6,line+1);
        oled_write_char(165,false);
        oled_write_char(166,false);
    }
    // TODO: handle muted state
    // oled_set_cursor(8,line);
    // oled_write_char(145,false);
    // oled_set_cursor(8,line+1);
    // oled_write_char(177,false);
    // oled_set_cursor(9,line);
    // if (!oled_state.muted) {
    //     oled_write_char(146,false);
    //     oled_set_cursor(9,line+1);
    //     oled_write_char(178,false);
    // } else {
    //     oled_write_char(147,false);
    //     oled_set_cursor(9,line+1);
    //     oled_write_char(179,false);
    // }
    oled_advance_page(false);
}

static inline void render_linebreak(uint8_t line) {
    oled_set_cursor(0, line);
    oled_write_P(PSTR("__________"), false);
}

static void render_wpm(uint8_t line) {
    oled_set_cursor(0, line);
    oled_write_P(PSTR("WPM:"), false);
    oled_write(get_u8_str(get_current_wpm(), ' '), false);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_90;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        render_layer(1);
        render_linebreak(2);
        // TODO: render keyboard layout
        // TODO: render clock
        render_linebreak(11);
        render_modifier_state(12);
        render_linebreak(14);
        render_wpm(15);
    } else {
        render_draw();
    }
    return false;
}
#endif


#ifdef LAYOUT_split_3x6_3_ex2
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3_ex2(
  //,--------------------------------------------------------------.  ,--------------------------------------------------------------.
        KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T, XXXXXXX,    XXXXXXX,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
        KC_ESC,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G, XXXXXXX,    XXXXXXX,    KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------+--------'  `--------+--------+--------+--------+--------+--------+--------|
       KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, RSFT_T(KC_ENT),
  //|--------+--------+--------+--------+--------+--------+--------.  ,--------+--------+--------+--------+--------+--------+--------|
                                          KC_LCTL, KC_LGUI, LT(1,KC_SPC),  LT(2,KC_SPC), MO(3), KC_LALT
                                      //`--------------------------'  `--------------------------'
  ),

  [1] = LAYOUT_split_3x6_3_ex2(
  //,--------------------------------------------------------------.  ,--------------------------------------------------------------.
       XXXXXXX,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5, XXXXXXX,    XXXXXXX,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
       XXXXXXX, XXXXXXX, KC_HOME, KC_PGDN, KC_PGUP,  KC_END, XXXXXXX,    XXXXXXX, KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, KC_PDOT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------'  `--------+--------+--------+--------+--------+--------+--------|
       _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
  //|--------+--------+--------+--------+--------+--------+--------.  ,--------+--------+--------+--------+--------+--------+--------|
                                          KC_LCTL, KC_LGUI, _______,    _______, _______, KC_LALT
                                      //`--------------------------'  `--------------------------'
  ),

  [2] = LAYOUT_split_3x6_3_ex2(
  //,--------------------------------------------------------------.  ,--------------------------------------------------------------.
        KC_TAB, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC, XXXXXXX,    XXXXXXX, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,  KC_GRV,
  //|--------+--------+--------+--------+--------+--------+--------'  `--------+--------+--------+--------+--------+--------+--------|
       _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
  //|--------+--------+--------+--------+--------+--------+--------.  ,--------+--------+--------+--------+--------+--------+--------|
                                          KC_LCTL, KC_LGUI, _______,    _______, _______, KC_LALT
                                      //`--------------------------'  `--------------------------'
  ),

  [3] = LAYOUT_split_3x6_3_ex2(
  //,--------------------------------------------------------------.  ,--------------------------------------------------------------.
       XXXXXXX,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5, XXXXXXX,    XXXXXXX,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_DEL,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
    S(KC_CAPS), XXXXXXX, RGB_TOG,  KC_PWR, KC_PSCR, KC_VOLU, XXXXXXX,    XXXXXXX, KC_MUTE, KC_MPRV, KC_MPLY, KC_MNXT, RALT(KC_GRV), RALT(KC_QUOT),
  //|--------+--------+--------+--------+--------+--------+--------'  `--------+--------+--------+--------+--------+--------+--------|
       _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLD,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------.  ,--------+--------+--------+--------+--------+--------+--------|
                                          KC_LCTL, KC_LGUI, XXXXXXX,    XXXXXXX, _______, KC_LALT
                                      //`--------------------------'  `--------------------------'
  )
};
#else
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, TL_LOWR,  KC_SPC,     KC_ENT, TL_UPPR, KC_RALT
                                      //`--------------------------'  `--------------------------'

  ),

  [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

  [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,  KC_GRV,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

  [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  )
};
#endif

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // Turn off LEDs for unused EX switch positions (OLED installed there)
    const uint8_t unused[][2] = {
        {0, 6}, // Left EX1
        {1, 6}, // Left EX2
        {4, 6}, // Right EX1
        {5, 6}, // Right EX2
    };
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t idx = g_led_config.matrix_co[unused[i][0]][unused[i][1]];
        if (idx != NO_LED) {
            RGB_MATRIX_INDICATOR_SET_COLOR(idx, 0, 0, 0);
        }
    }
    return false;
}

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
  [0] = { ENCODER_CCW_CW(RGB_MOD, RGB_RMOD), ENCODER_CCW_CW(RGB_HUI, RGB_HUD), ENCODER_CCW_CW(RGB_VAI, RGB_VAD), ENCODER_CCW_CW(RGB_SAI, RGB_SAD), },
  [1] = { ENCODER_CCW_CW(RGB_MOD, RGB_RMOD), ENCODER_CCW_CW(RGB_HUI, RGB_HUD), ENCODER_CCW_CW(RGB_VAI, RGB_VAD), ENCODER_CCW_CW(RGB_SAI, RGB_SAD), },
  [2] = { ENCODER_CCW_CW(RGB_MOD, RGB_RMOD), ENCODER_CCW_CW(RGB_HUI, RGB_HUD), ENCODER_CCW_CW(RGB_VAI, RGB_VAD), ENCODER_CCW_CW(RGB_SAI, RGB_SAD), },
  [3] = { ENCODER_CCW_CW(RGB_MOD, RGB_RMOD), ENCODER_CCW_CW(RGB_HUI, RGB_HUD), ENCODER_CCW_CW(RGB_VAI, RGB_VAD), ENCODER_CCW_CW(RGB_SAI, RGB_SAD), },
};
#endif
