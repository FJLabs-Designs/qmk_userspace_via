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
        uint8_t caps_led_state : 8;
        uint8_t scroll_led_state : 8;
    };
} user_config_t;
user_config_t user_config; 

uint8_t socd_state = 0; 
uint8_t caps_led_state = 1; 
uint8_t scroll_led_state = 1; 
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
    caps_led_state = user_config.caps_led_state;
    scroll_led_state = user_config.scroll_led_state;
};

// When the EEPROM gets forcefully reset, set the initialization value
void eeconfig_init_user(void) {
    user_config.raw = 0;
    user_config.socd_state = 0;
    user_config.caps_led_state = 1;
    user_config.scroll_led_state = 1;
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
    id_socd_state = 1,
    id_caps_led_state = 2,
    id_scroll_led_state = 3
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

        case id_caps_led_state:
        {
            caps_led_state = *value_data;
            user_config.caps_led_state = *value_data;
            eeconfig_update_user(user_config.raw);
        }

        case id_scroll_led_state:
        {
            scroll_led_state = *value_data;
            user_config.scroll_led_state = *value_data;
            eeconfig_update_user(user_config.raw);
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

        case id_caps_led_state:
        {
            *value_data = caps_led_state;
            break;
        }

        case id_scroll_led_state:
        {
            *value_data = scroll_led_state;
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

/**
 * Helper function to handle SOCD (Simultaneous Opposite Cardinal Direction) logic
 *
 * @param keycode The keycode being processed
 * @param record Pointer to the keyrecord_t structure
 * @param key1 First key in the pair (e.g., KC_W)
 * @param key2 Second key in the pair (e.g., KC_S)
 * @param flag1 Pointer to the flag for the first key (e.g., &w_down)
 * @param flag2 Pointer to the flag for the second key (e.g., &s_down)
 * @return true if the key was handled, false otherwise
 */
bool handle_socd_pair(uint16_t keycode, keyrecord_t *record, uint16_t key1, uint16_t key2, bool *flag1, bool *flag2) {
    if (keycode == key1) {
        if (record->event.pressed) {
            if (*flag2) {
                unregister_code(key2);
            }
            register_code(key1);
            *flag1 = true;
        } else {
            unregister_code(key1);
            *flag1 = false;
            
            if (*flag2) {
                register_code(key2);
            }
        }
        return true;
    } else if (keycode == key2) {
        if (record->event.pressed) {
            if (*flag1) {
                unregister_code(key1);
            }
            register_code(key2);
            *flag2 = true;
        } else {
            unregister_code(key2);
            *flag2 = false;
            
            if (*flag1) {
                register_code(key2);
            }
        }
        return true;
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // If SOCD is OFF, use default handling
    if (socd_state == 0) {
        return true;
    }
    
    bool handled = false;
    
    // Handle based on SOCD state
    switch (socd_state) {
        case 1: // FULL - Handle both W-S and A-D pairs
            handled = handle_socd_pair(keycode, record, KC_W, KC_S, &w_down, &s_down);
            if (!handled) {
                handled = handle_socd_pair(keycode, record, KC_A, KC_D, &a_down, &d_down);
            }
            break;
            
        case 2: // HORIZONTAL - Handle only A-D pair
            handled = handle_socd_pair(keycode, record, KC_A, KC_D, &a_down, &d_down);
            break;
            
        case 3: // VERTICAL - Handle only W-S pair
            handled = handle_socd_pair(keycode, record, KC_W, KC_S, &w_down, &s_down);
            break;
            
        case 4: // OSU - Handle Z-X pair (using w_down and s_down flags)
            handled = handle_socd_pair(keycode, record, KC_Z, KC_X, &w_down, &s_down);
            break;
    }
    
    // Return false if we handled the key, true otherwise
    return !handled;
}


bool led_update_user(led_t led_state) {
    // DEBUG: Print LED state values
    dprintf("LED Update - Caps: %d, Scroll: %d, Caps State: %d, Scroll State: %d\n",
            led_state.caps_lock, led_state.scroll_lock, caps_led_state, scroll_led_state);
    
    // Only proceed with our custom handling if the user function returns true
    
        // Configure pins as outputs
        setPinOutput(B7); // Caps Lock LED
        setPinOutput(B6); // Scroll Lock LED
        
        // Handle Caps Lock LED
        if (caps_led_state == 1 && led_state.caps_lock) {
        //if (led_state.caps_lock) {
            writePinHigh(B7); // Turn on Caps Lock LED at full brightness
        } else {
            writePinLow(B7);  // Turn off Caps Lock LED
        }
        
        // Handle Scroll Lock LED
        if (scroll_led_state == 1 && led_state.scroll_lock) {
        //if (led_state.scroll_lock) {
            writePinHigh(B6); // Turn on Scroll Lock LED at full brightness
        } else {
            writePinLow(B6);  // Turn off Scroll Lock LED
        }

    // Return true to allow default LED handling to occur as well
    return true;
}