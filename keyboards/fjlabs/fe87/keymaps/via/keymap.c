/*
Copyright 2021 <felix@fjlaboratories.com>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

enum layers {
    _LAYER0,
    _LAYER1
};

bool w_down = false;
bool a_down = false;
bool s_down = false;
bool d_down = false;

typedef union {
    uint32_t raw;
    struct {
        uint8_t socd_state : 8; // Hold only the SOCD function state.
    };
} user_config_t;
user_config_t user_config; 

uint8_t socd_state = 0; 
// 0 == Off
// 1 == Full [W-S // S-W // A-D // D-A]
// 2 == Horizontal [A-D // D-A]
// 3 == Vertical [W-S // S-W]
// 4 == OSU [Z-X // X-Z]

// On keyboard initialization, pull the EEPROM values
void keyboard_post_init_user(void) {
    // Call the keymap level matrix init.
    user_config.raw = eeconfig_read_user();
    socd_state = user_config.socd_state;
};

// When the EEPROM gets forcefully reset, set the initialization value
void eeconfig_init_user(void) {
    user_config.raw = 0;
    user_config.socd_state = 0;
    eeconfig_update_user(user_config.raw);
}



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_LAYER0] = LAYOUT_tkl_all(
        KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,            KC_PSCR, KC_SCRL, KC_PAUS,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,   KC_DEL, KC_INS,  KC_HOME, KC_PGUP,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,           KC_DEL,  KC_END,  KC_PGDN,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,                
        KC_LSFT, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_RSFT,                    KC_UP,
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                             KC_RALT, MO(1),   KC_RWIN, KC_RCTL,                    KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [_LAYER1] = LAYOUT_tkl_all(
        KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,   KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,            KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS,                            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,            KC_TRNS, KC_TRNS, KC_TRNS
    )
};


// ====== Process VIA Custom UI ======
// enum for the Value ID of the SOCD control
enum via_socd_control {
    id_socd_state = 1
};

// Define the function to get the state for SOCD value
void socd_state_set_value ( uint8_t *data ) {
    // data = [ value_id, value_data ]
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch ( *value_id )
    {
        case id_socd_state:
        {
            socd_state = *value_data;
            user_config.socd_state = *value_data;
            eeconfig_update_user(user_config.raw); // Going to set it here instead of separately in the VIA save function. No point since this value is not being toggled very often
            break;
        }
    }
}

// Define the function to read the SOCD state [so that VIA UI knows what to display]
void socd_state_get_value( uint8_t *data ) {
    // data = [ value_id, value_data ]
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch ( *value_id )
    {
        case id_socd_state:
        {
            *value_data = socd_state;
            break;
        }
    }
}

// Boilerplate from VIA Custom UI Documentation. Listen on the custom command channel, 
void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    // data = [ command_id, channel_id, value_id, value_data ]
    uint8_t *command_id        = &(data[0]);
    uint8_t *channel_id        = &(data[1]);
    uint8_t *value_id_and_data = &(data[2]);

    if ( *channel_id == id_custom_channel ) {
        switch ( *command_id )
        {
            case id_custom_set_value:
            {
                socd_state_set_value(value_id_and_data);
                break;
            }
            case id_custom_get_value:
            {
                socd_state_get_value(value_id_and_data);
                break;
            }
            case id_custom_save:
            {
                break;
            }
            default:
            {
                *command_id = id_unhandled;
                break;
            }
        }
        return;
    }

    // Return the unhandled state
    *command_id = id_unhandled;

    // DO NOT call raw_hid_send(data,length) here, let caller do this
}





bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (socd_state == 0) { // SOCD is OFF

    } else if (socd_state == 1) { // SOCD is FULL
        switch (keycode) {
            case KC_W:
                if (record->event.pressed) {
                    if (s_down) {
                        unregister_code(KC_S);
                    }
                    register_code(KC_W);
                    w_down = true;
                } else {
                    unregister_code(KC_W);
                    w_down = false;

                    if (s_down) {
                        register_code(KC_S);
                    }

                }
                return false;
                break;
            case KC_A:
                if (record->event.pressed) {
                    if (d_down) {
                        unregister_code(KC_D);
                    }
                    register_code(KC_A);
                    a_down = true;
                } else {
                    unregister_code(KC_A);
                    a_down = false;

                    if (d_down) {
                        register_code(KC_D);
                    }

                }
                return false;
                break;
            case KC_S:
                if (record->event.pressed) {
                    if (w_down) {
                        unregister_code(KC_W);
                    }
                    register_code(KC_S);
                    s_down = true;
                } else {
                    unregister_code(KC_S);
                    s_down = false;

                    if (w_down) {
                        register_code(KC_W);
                    }

                }
                return false;
                break;
            case KC_D:
                if (record->event.pressed) {
                    if (a_down) {
                        unregister_code(KC_A);
                    }
                    register_code(KC_D);
                    d_down = true;
                } else {
                    unregister_code(KC_D);
                    d_down = false;

                    if (a_down) {
                        register_code(KC_A);
                    }
                }
                return false;
                break;
        }
    } else if (socd_state == 2) { // SOCD is HORIZONTAL [A-D]
        switch (keycode) {
            case KC_A:
                if (record->event.pressed) {
                    if (d_down) {
                        unregister_code(KC_D);
                    }
                    register_code(KC_A);
                    a_down = true;
                } else {
                    unregister_code(KC_A);
                    a_down = false;

                    if (d_down) {
                        register_code(KC_D);
                    }

                }
                return false;
                break;
            case KC_D:
                if (record->event.pressed) {
                    if (a_down) {
                        unregister_code(KC_A);
                    }
                    register_code(KC_D);
                    d_down = true;
                } else {
                    unregister_code(KC_D);
                    d_down = false;

                    if (a_down) {
                        register_code(KC_A);
                    }
                }
                return false;
                break;
        }
    } else if (socd_state == 3) { // SOCD is VERTICAL [W-S]
        switch (keycode) {
            case KC_W:
                if (record->event.pressed) {
                    if (s_down) {
                        unregister_code(KC_S);
                    }
                    register_code(KC_W);
                    w_down = true;
                } else {
                    unregister_code(KC_W);
                    w_down = false;

                    if (s_down) {
                        register_code(KC_S);
                    }
                }
                return false;
                break;
            case KC_S:
                if (record->event.pressed) {
                    if (w_down) {
                        unregister_code(KC_W);
                    }
                    register_code(KC_S);
                    s_down = true;
                } else {
                    unregister_code(KC_S);
                    s_down = false;

                    if (w_down) {
                        register_code(KC_W);
                    }
                }
                return false;
                break;
        }
    } else if (socd_state == 4) { // SOCD is OSU [Z-X]
        switch (keycode) {
            case KC_Z:
                if (record->event.pressed) {
                    if (s_down) {
                        unregister_code(KC_X);
                    }
                    register_code(KC_Z);
                    w_down = true;
                } else {
                    unregister_code(KC_Z);
                    w_down = false;

                    if (s_down) {
                        register_code(KC_X);
                    }
                }
                return false;
                break;
            case KC_X:
                if (record->event.pressed) {
                    if (w_down) {
                        unregister_code(KC_Z);
                    }
                    register_code(KC_X);
                    s_down = true;
                } else {
                    unregister_code(KC_X);
                    s_down = false;

                    if (w_down) {
                        register_code(KC_Z);
                    }
                }
                return false;
                break;
        }
    }
    return true;
            
}