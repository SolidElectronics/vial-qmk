 /* This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */
#include QMK_KEYBOARD_H

enum layers{
  _MAIN,
  _ALT
};

enum jiggle_keycode {
	M_JIGL = QK_KB_0,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	// Layer 0
	// Function key: Tap to toggle mouse jiggle, hold to temporarily switch layer
	[_MAIN] = LAYOUT(
		MO(_ALT), QK_MACRO_0, QK_MACRO_1,
		QK_MACRO_2, QK_MACRO_3, QK_MACRO_4,
		QK_MACRO_5, QK_MACRO_6, QK_MACRO_7
	),
	[_ALT] = LAYOUT(
		KC_TRNS, RGB_VAI, RGB_VAD,
		M_JIGL, RGB_MOD, RGB_RMOD,
		KC_TRNS, KC_TRNS, KC_TRNS
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

bool is_mouse_jiggle_active = false;
bool mouse_jiggle_direction = false;		// used to alternate direction
uint16_t mouse_jiggle_frequency = 60000;	// how often to move the mouse (ms)
uint16_t mouse_jiggle_timer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case M_JIGL:
			if (record->event.pressed) {
				is_mouse_jiggle_active = !is_mouse_jiggle_active;
				// Confirm it's doing something - this could come out.
				if (is_mouse_jiggle_active) {
					tap_code16(KC_1);
				} else {
					tap_code16(KC_0);
				}
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
	// Loop through all keys to see if any need their LEDs set
	uint8_t current_layer = get_highest_layer(layer_state);
	uint8_t current_brightness = rgb_matrix_get_val();
	for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
		for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
			uint8_t index = g_led_config.matrix_co[row][col];
			// Mouse jiggle key light indicates status (specific color if on)
			if (index >= led_min && index < led_max && index != NO_LED &&
                keymap_key_to_keycode(current_layer, (keypos_t){col,row}) == QK_KB_0) {
				if (is_mouse_jiggle_active) {
					// Apply global brightness to indicator color
					rgb_matrix_set_color(index,
						(uint8_t) 0x20 * current_brightness / 0xFF,	// R
						(uint8_t) 0x80 * current_brightness / 0xFF,	// G
						(uint8_t) 0x20 * current_brightness	/ 0xFF	// B
					);
				}
			}
		}
	}
    return false;
}
