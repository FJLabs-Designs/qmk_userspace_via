/*
Copyright 2024 <hello@keebfront.com>
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
#include "analog.h"
#include "string.h"

enum layers {
    _LAYER0,
    _LAYER1,
    _LAYER2,
    _LAYER3
};

enum custom_keycodes {
    A_CW = QK_KB_0,
    A_CCW
};

// Set up EEPROM. Mostly boilerplate from QMK docs
typedef union {
    uint32_t raw;
    struct {
        uint8_t slider_func_state : 8; // Hold only the slider function state. Will be a low value integer, so 8 bits will be used to store.
        uint8_t slider_midi_by_layer_state : 8; // Hold a 0 or a 1 for whether the slider MIDI CC channel will change by layer
        uint8_t slider_smoothing_state : 8; // Hold 0-4 for the slider smoothing value
        uint8_t socd_state : 8; // Hold 0-4 for the SOCD state
    };
} user_config_t;
user_config_t user_config; 

// Slider Variable init
uint8_t slider_func = 0;
uint8_t slider_midi_by_layer = 0;
uint8_t slider_smoothing = 0;

// SOCD Variable init
bool w_down = false;
bool a_down = false;
bool s_down = false;
bool d_down = false;
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
    // Initialize the default values from EEPROM
    slider_func = user_config.slider_func_state;
    slider_midi_by_layer = user_config.slider_midi_by_layer_state;
    slider_smoothing = user_config.slider_smoothing_state;
    socd_state = user_config.socd_state;
};

// When the EEPROM gets forcefully reset, set the initialization value
void eeconfig_init_user(void) {
    user_config.raw = 0;
    user_config.slider_func_state = 0;
    user_config.slider_midi_by_layer_state = 0;
    user_config.slider_smoothing_state = 0;
    user_config.socd_state = 0;
    eeconfig_update_user(user_config.raw);
}

// ===================================
// ======= MATRIX AND ENCODER ========
// ===================================

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_LAYER0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [_LAYER1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [_LAYER2] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [_LAYER3] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)}
};
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_LAYER0] = LAYOUT_all(
        KC_MUTE, QK_GESC, KC_1,    KC_2,    KC_3, KC_4, KC_5, KC_6,  KC_7, KC_8, KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL, KC_HOME,
                 KC_TAB,  KC_Q,    KC_W,    KC_E, KC_R, KC_T, KC_Y,  KC_U, KC_I, KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,         KC_PGUP, 
                 KC_CAPS, KC_A,    KC_S,    KC_D, KC_F, KC_G, KC_H,  KC_J, KC_K, KC_L,    KC_SCLN, KC_QUOT, KC_ENT,                   KC_PGDN, 
                 KC_LSFT, KC_NUBS, KC_Z,    KC_X, KC_C, KC_V, KC_B,  KC_N, KC_M, KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP, 
                 KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,            KC_RALT, MO(1),   KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [_LAYER1] = LAYOUT_all(
        _______, QK_GESC, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, KC_END, 
                 _______, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______, KC_PSCR, KC_SCRL, KC_PAUS, QK_BOOT, _______,
                 _______, RGB_SPI, RGB_SPD, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
                 _______, _______, _______, _______, _______, _______, _______, NK_TOGG, _______, _______, _______, _______, _______, KC_VOLU,  
                 _______, _______, _______,                            _______,                   _______, _______, KC_MPRV, KC_VOLD, KC_MNXT
    ),

    [_LAYER2] = LAYOUT_all(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

    [_LAYER3] = LAYOUT_all(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

};

// -----------------


// ===================================
// ======= MIDI ENCODER CODES ========
// ======= SOCD KEYS HANDLING ========
// ===================================

/* Configure custom MIDI Keycodes that will be placed on USER_00 and USER_01 */
extern MidiDevice midi_device;
#define MIDI_CC_OFF 0
#define MIDI_CC_ON 127

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (socd_state == 0) { // SOCD is OFF
        switch (keycode) {
            case A_CW:
                if (record->event.pressed) {
                    midi_send_cc(&midi_device, midi_config.channel, 89, 1);
                }
                return true;
            case A_CCW:
                if (record->event.pressed) {
                    midi_send_cc(&midi_device, midi_config.channel, 89, 127);
                }
                return true;
            default:
                return true;
        }
    } else if (socd_state == 1) { // SOCD is FULL
        switch (keycode) {
            case A_CW:
                if (record->event.pressed) {
                    midi_send_cc(&midi_device, midi_config.channel, 89, 1);
                }
                return true;
            case A_CCW:
                if (record->event.pressed) {
                    midi_send_cc(&midi_device, midi_config.channel, 89, 127);
                }
                return true;
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
            case A_CW:
                if (record->event.pressed) {
                    midi_send_cc(&midi_device, midi_config.channel, 89, 1);
                }
                return true;
            case A_CCW:
                if (record->event.pressed) {
                    midi_send_cc(&midi_device, midi_config.channel, 89, 127);
                }
                return true;
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
            case A_CW:
                if (record->event.pressed) {
                    midi_send_cc(&midi_device, midi_config.channel, 89, 1);
                }
                return true;
            case A_CCW:
                if (record->event.pressed) {
                    midi_send_cc(&midi_device, midi_config.channel, 89, 127);
                }
                return true;
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
            case A_CW:
                if (record->event.pressed) {
                    midi_send_cc(&midi_device, midi_config.channel, 89, 1);
                }
                return true;
            case A_CCW:
                if (record->event.pressed) {
                    midi_send_cc(&midi_device, midi_config.channel, 89, 127);
                }
                return true;
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

// ------------------


// ====== Process VIA Custom UI ======
// Establish enums for the multiple possible configurations of ids that could arise.
// Each unique VIA configuration requires a separate enum value, which is the unique id
// that is used to define that value in the other functions throughout.
enum via_slider_control {
    id_slider_func = 1,
    id_slider_midi_by_layer = 2,
    id_slider_smoothing = 3,
    id_socd_state = 4
};

// Define the function to set the slider value
void slider_func_set_value ( uint8_t *data ) {
    // data = [ value_id, value_data ]
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch ( *value_id )
    {
        // Pull the value from the VIA data that is being passed. Each `case` iterates through the different options that were passed in from VIA, and the identification is the enum id defined above. Pull each VIA config value into the appropriate global value in the keyboard, and then update the EEPROM with the value.
        case id_slider_func:
        {
            slider_func = *value_data;
            user_config.slider_func_state = *value_data;
            eeconfig_update_user(user_config.raw); // Going to set it here instead of separately in the VIA save function. No point since this value is not being toggled very often
            break;
        }

        case id_slider_midi_by_layer:
        {
            slider_midi_by_layer = *value_data;
            user_config.slider_midi_by_layer_state = *value_data;
            eeconfig_update_user(user_config.raw);
            break;
        }

        case id_slider_smoothing:
        {
            slider_smoothing = *value_data;
            user_config.slider_smoothing_state = *value_data;
            eeconfig_update_user(user_config.raw);
            break;
        }

        case id_socd_state:
        {
            socd_state = *value_data;
            user_config.socd_state = *value_data;
            eeconfig_update_user(user_config.raw);
            break;
        }
    }
}

// Define the function to read the slider value [so that VIA UI knows what to display]
void slider_func_get_value( uint8_t *data ) {
    // data = [ value_id, value_data ]
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch ( *value_id )
    {
        // Same as above but in the reverse case. We send the data to the VIA web app for each of the enums.
        case id_slider_func:
        {
            *value_data = slider_func;
            break;
        }

        case id_slider_midi_by_layer:
        {
            *value_data = slider_midi_by_layer;
            break;
        }

        case id_slider_smoothing:
        {
            *value_data = slider_smoothing;
            break;
        }

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
                // Call the function to pull values down
                slider_func_set_value(value_id_and_data);
                break;
            }
            case id_custom_get_value:
            {
                // Call the function to push values up
                slider_func_get_value(value_id_and_data);
                break;
            }
            case id_custom_save:
            {
                //buttglow_config_save(); // Not doing this
                break;
            }
            default:
            {
                // Unhandled message.
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


// ==================================
// ====== MIDI Slider controls ======
// ==================================

// For RGB Brightness
uint8_t last_val = 0;
uint8_t rgb_hue = 0;
uint8_t rgb_sat = 0;
// For MIDI by Layer
uint8_t midi_cc_channel = 90; // For the default MIDI CC channel
// For Slider Smoothing
uint16_t accumulator = 0;
uint8_t current_val = 0;

void slider(void) {

    // Slider smoothing algorithm
    // https://electronics.stackexchange.com/questions/176721/how-to-smooth-analog-data
    if ( slider_smoothing == 0 ) {
        current_val = analogReadPin(SLIDER_PINA) >>3; // Bit shift 2^10 [1024] to 2^7 [128]
    } else {
        uint8_t raw_val = analogReadPin(SLIDER_PINA) >>3;
        accumulator = accumulator - (accumulator >> slider_smoothing) + raw_val;
        current_val = accumulator >> slider_smoothing;
    }
    
    // Slider Functions
    if ( last_val - current_val < -1 || last_val - current_val > 1 ) { 

        // Underglow RGB Brightness
        if ( slider_func == 0 ) {
            uint8_t rounded_val = 0;
            uint8_t round_coefficient = 5; // Set round-to-zero value target
            rgb_hue = rgblight_get_hue(); // Pull current hue and saturation values since we're just adjusting brightness
            rgb_sat = rgblight_get_sat();

            // Since the lower end range of the slider can be a little bit noisy, it's going to make the zero-value a little hard to hit when it bounces around between 0-1-2. Better off to round any super low values to zero so the lights will affirmatively turn off at lower values.
            if ( current_val < round_coefficient ) {
                rounded_val = 0;
            } else {
                rounded_val = current_val;
            }

            rgblight_sethsv(rgb_hue, rgb_sat, rounded_val * 2); // At 8 bits, it's going to be 0-128, so double to get full range.
        }

        // MIDI CC Channel Data
        else if ( slider_func == 1 ) {
            if ( slider_midi_by_layer == 0 ) {
                midi_send_cc(&midi_device, 0, 90, current_val );    
            } else {
                midi_send_cc(&midi_device, 0, midi_cc_channel, current_val );
            }
        }

        // Layer shift between layers 1 - 4 
        else if ( slider_func == 2 ) {
            layer_move(current_val >>5); // Bit shift 2^7 to 2^2
        }

        else {
            return;
        }

        last_val = current_val;
    }
}
 
void housekeeping_task_user(void) {
    slider();
}

// ---------------------------------


// =================================
// ======= MIDI CC BY LAYER ========
// ======= LAYER ID HANDLER ========
// =================================
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _LAYER1:
        midi_cc_channel = 88;
        break;
    case _LAYER2:
        midi_cc_channel = 87;
        break;
    case _LAYER3:
        midi_cc_channel = 86;
        break;
    default: //  for any other layers, or the default layer
        midi_cc_channel = 90;
        break;
    }
  return state;
}

// ---------------------------------