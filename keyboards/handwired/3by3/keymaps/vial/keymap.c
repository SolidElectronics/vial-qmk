// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

enum layers{
	_MAIN,
	_MACRO1,
	_MACRO2,
	_CFG
};

enum jiggle_keycode {
	M_JIGL = QK_KB_0,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	// Layer 0: Media and layer select
	[_MAIN] = LAYOUT(
		KC_MPRV, KC_MPLY, KC_MNXT,
		KC_MRWD, KC_MSTP, KC_MFFD,
		MO(_CFG), MO(_MACRO2), MO(_MACRO1)
	),
	// Layer 1: Macro set 1
	[_MACRO1] = LAYOUT(
		QK_MACRO_0, QK_MACRO_1, QK_MACRO_2,
		QK_MACRO_3, QK_MACRO_4, QK_MACRO_5,
		QK_MACRO_6, QK_MACRO_7, KC_TRNS
	),
	// Layer 2: Macro set 2
	[_MACRO2] = LAYOUT(
		QK_MACRO_8, QK_MACRO_9, QK_MACRO_10,
		QK_MACRO_11, QK_MACRO_12, QK_MACRO_13,
		QK_MACRO_14, KC_TRNS, QK_MACRO_16
	),
	// Layer 3: Pad config
	[_CFG] = LAYOUT(
		M_JIGL, RGB_VAI, RGB_VAD,
		QK_BOOTLOADER, RGB_MOD, RGB_RMOD,
		KC_TRNS, KC_SYSTEM_WAKE, RGB_TOG
	)
};

led_config_t g_led_config = { {
	// Wiring order
	{ 0, 1, 2 },
	{ 3, 4, 5 },
	{ 6, 7, 8 }
}, {
	// Physical location
	{0,0}, {112,0}, {224,0},
	{0,32}, {112,32}, {224,32},
	{0,64}, {112,32}, {224,64}
}, {
	// Flags
	4, 4, 8, 4, 4, 4, 4, 4, 4
} };

bool is_mouse_jiggle_active = true;			// Default to on when started up
bool mouse_jiggle_direction = false;		// used to alternate direction
uint16_t mouse_jiggle_frequency = 60000;	// how often to move the mouse (ms)
uint16_t mouse_jiggle_timer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case M_JIGL:
			if (record->event.pressed) {
				is_mouse_jiggle_active = !is_mouse_jiggle_active;
			}
		break;
	}
	return true;
}

void matrix_scan_user(void) {
	if (mouse_jiggle_timer == 0) mouse_jiggle_timer = timer_read();

	if (is_mouse_jiggle_active) {
		if (timer_elapsed(mouse_jiggle_timer) > mouse_jiggle_frequency) {
			mouse_jiggle_timer = timer_read();
			if (mouse_jiggle_direction) {
				tap_code(KC_MS_LEFT);
			} else {
				tap_code(KC_MS_RIGHT);
			}
			mouse_jiggle_direction = !mouse_jiggle_direction;
		}
	}
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	uint8_t current_layer = get_highest_layer(layer_state);
	uint8_t current_brightness = rgb_matrix_get_val();
	// Loop through all keys to see if any need their LEDs set
	for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
		for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
			uint8_t index = g_led_config.matrix_co[row][col];
			// Mouse jiggle key light indicates status 
			// Put this on a temporary layer or modifier so it doesn't show all the time.
			if (index >= led_min && index < led_max && index != NO_LED &&
			keymap_key_to_keycode(current_layer, (keypos_t){col,row}) == QK_KB_0) {
				if (is_mouse_jiggle_active) {
					rgb_matrix_set_color(index,
						(uint8_t) 0x20 * current_brightness / 0xFF,	// R
						(uint8_t) 0x80 * current_brightness / 0xFF,	// G
						(uint8_t) 0x20 * current_brightness	/ 0xFF	// B
					);
				} else {
					rgb_matrix_set_color(index,
						(uint8_t) 0x80 * current_brightness / 0xFF,	// R
						(uint8_t) 0x20 * current_brightness / 0xFF,	// G
						(uint8_t) 0x20 * current_brightness	/ 0xFF	// B
					);
				}
			}
			// Bootloader button shows black
			// Put this on a temporary layer or modifier so it doesn't show all the time.
			if (index >= led_min && index < led_max && index != NO_LED &&
			keymap_key_to_keycode(current_layer, (keypos_t){col,row}) == QK_BOOTLOADER) {
				rgb_matrix_set_color(index, 0,0,0);
			}
		}
	}
	return false;
}
