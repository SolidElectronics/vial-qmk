// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

enum layers{
	_MAIN,
	_FN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_MAIN] = LAYOUT(
		KC_A, KC_B, KC_C,
		KC_D, KC_E, KC_F,
		KC_G, KC_H, KC_I
	),
	[_FN] = LAYOUT(
		KC_F13, KC_F14, KC_F15,
		KC_F16, KC_F17, KC_F18,
		KC_F19, KC_F20, KC_F21
	)
};
