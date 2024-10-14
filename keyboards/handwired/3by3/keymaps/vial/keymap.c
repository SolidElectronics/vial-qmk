 /* Copyright 2024 SolidElectronics
  *
  * This program is free software: you can redistribute it and/or modify
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
  _MAIN
};

enum jiggle_keycode {
	M_JIGL = QK_KB_0,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_MAIN] = LAYOUT(
    M_JIGL, QK_MACRO_1, QK_MACRO_2,
    QK_MACRO_3, QK_MACRO_4, QK_MACRO_5,
    QK_MACRO_6, QK_MACRO_7, QK_MACRO_8
  )
};

led_config_t g_led_config = { {
	{ 0, 1, 2 },
	{ 3, 4, 5 },
	{ 6, 7, 8 }
}, {
	{0,0}, {112,0}, {224,0},
	{0,32}, {112,32}, {224,32},
	{0,64}, {112,32}, {224,64}
}, {
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
//	if (host_keyboard_led_state().caps_lock) {
//		for (uint8_t i = led_min; i < led_max; i++) {
//			if (g_led_config.flags[i] & LED_FLAG_KEYLIGHT) {
//				rgb_matrix_set_color(i, RGB_RED);
//			}
//		}
//	}
	if (is_mouse_jiggle_active) {
		rgb_matrix_set_color(2, RGB_RED);
	} else {
		rgb_matrix_set_color(2, RGB_GREEN);
	}
    return false;
}

