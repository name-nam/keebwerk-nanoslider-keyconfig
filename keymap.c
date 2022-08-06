/* 
keymap.c

Copyright (c) 2022 name-nam

This software is released under the GNU LGPLv3.
see https://github.com/name-nam/keebwerk-nanoslider-keyconfig/blob/master/LICENSE
 */

/*
This program is a modified version of https://github.com/jesusvallejo/nanokeymaps/blob/master/keymap.c
The section I have added is shown as '-----add'.
*/

#include QMK_KEYBOARD_H
#include "analog.h"
#include "qmk_midi.h"


/*
This keymap works with midi2vol: linux version  https://github.com/jesusvallejo/Midi2Vol-Linux  
and windows version https://github.com/jesusvallejo/Midi2Vol

Enables the user to change the volume on the host computer if running midi2vol software.

-----add
If changing the volume with the MIDI slider does not work , I recommend to use MIDI Mixer :https://www.midi-mixer.com
(I used it because midi2vol did not work well)

If you have any questions , you should read the reedme.
Or please contact me on Twitter : @name__nam
-----add

Example to extend usability is provided check VLC in: enum custom_keycodes {} and bool process_record_user(uint16_t keycode, keyrecord_t *record) {}
Remember to give allways the new volume control a hex value not used by other volume control, 
and add it to the midi2vol software.
Check respective midi2vol readme on how to do so.In linux change config.json,in windows you can use configuration option.

A compiled version of this keymap is provided in here: https://github.com/jesusvallejo/nanokeymaps/

*/

uint8_t midi2vol = 0x3E;

// Defines names for use in layer keycodes and the keymap

#define _DEF 0

#define _VOL 1

#define _MEDIA 2

#define _EXFC 3

#define _MFC 4

#define _EDIT 5

#define _RESET 6

#define _TOOGLE 7



// Defines the keycodes used by our macros in process_record_user

//-----add
// When using MIDI Mixer, this name is no longer relevant.
//-----add

enum custom_keycodes { // In order to add more volume controls, add here a custom keycode, ex: VLC
    DEFAULT= SAFE_RANGE,SPOTIFY,DISCORD,CHROME,VLC
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_DEF] = LAYOUT(
        MO(_TOOGLE),
        KC_F18, KC_F19, KC_F20,
        KC_F21, LCTL(LSFT(KC_M)), LCTL(LSFT(KC_D)), KC_F24
        //-----add
        //LCTL(LSFT(KC_M)) is Discord mute button, LCTL(LSFT(KC_D)) is Discord unmute button.
        //-----add
    ),
    [_VOL] = LAYOUT(
        MO(_TOOGLE),
        SPOTIFY, DISCORD, CHROME,
        VLC, LCTL(LSFT(KC_M)), KC_MUTE, DEFAULT      
    ),
    [_MEDIA] = LAYOUT(
        MO(_TOOGLE),
        KC_F13, KC_F14, KC_F15,
        KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE
    ),
    [_EXFC] = LAYOUT(
        MO(_TOOGLE),
        LALT(KC_1), LALT(KC_2), LALT(KC_3),
        LALT(KC_4), LALT(KC_5), LALT(KC_6), LALT(KC_7)
        //-----add
        //LALT(KC_1 "or any" ) is holding down the ALT key and press the 1 "or any" key.
        //-----add
    ),
    [_MFC] = LAYOUT(
        MO(_TOOGLE),
        KC_INS, KC_HOME, KC_PGUP,
        KC_DELETE, KC_END, KC_PGDN, KC_NO
    ),
    [_EDIT] = LAYOUT(
        MO(_TOOGLE),
        KC_CUT, KC_COPY, KC_PASTE,
        KC_UNDO, KC_AGIN, KC_NO, KC_NO
    ),
    [_RESET] = LAYOUT(
        MO(_TOOGLE),
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, RESET 
    ),
    [_TOOGLE] = LAYOUT(
        MO(_TOOGLE),
        TO(_DEF),    TO(_VOL),    TO(_MEDIA),
        TO(_EXFC),    TO(_MFC),    TO(_EDIT),    TO(_RESET)
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) { // In order to add more volume controls, add here code to detect custom keycode, ex: VLC
    switch (keycode) {
        case DEFAULT:
            if (record->event.pressed) {
                // when keycode DEFAULT is pressed
                midi2vol= 0x3E;
            } else {
                // when keycode DEFAULT is released
            }
            break;
        case SPOTIFY:
            if (record->event.pressed) {
                midi2vol= 0x3F;
            } else {
            }
            break;
        case DISCORD:
            if (record->event.pressed) {
                midi2vol= 0x40;
            } else {
            }
            break;
        case CHROME:
            if (record->event.pressed) {
                midi2vol= 0x41;
            } else {
            }
            break;
        
        case VLC:
            if (record->event.pressed) {
                midi2vol= 0x42;
            } else {
            }
            break;
        
    }
    return true;
}

uint8_t divisor = 0;

//-----add
//define tmp to store the previous midi2vol value.
//-----add
int tmp1,tmp2,tmp3;

void slider(void) {
    if (divisor++) {
        return;
    }

    //-----add
    /*
    If the MIDI slider is not moving, no value is sent.
    In other words, it sends only while it is moving.
    This is to prevent the MIDI Mixer overlay from continuing to display.
    If you were to send every time, the overlay would continue to appear forever.
    Also,if slider between previous value and next value , no value is sent ,because the overlay would continue to appear forever.
    ex) Back and forth between 4E and 4F.
    */
    if(tmp3==(analogReadPin(SLIDER_PIN) >> 3)){
        return;
    }else if(tmp1==(analogReadPin(SLIDER_PIN) >> 3)-1){
        return;
    }else if(tmp2==(analogReadPin(SLIDER_PIN) >> 3)+1){
        return;
    }else{
        midi_send_cc(&midi_device, 2, midi2vol, 0x7F - (analogReadPin(SLIDER_PIN) >> 3));
        tmp1=(analogReadPin(SLIDER_PIN) >> 3)-1;
        tmp2=(analogReadPin(SLIDER_PIN) >> 3)+1;
        tmp3=(analogReadPin(SLIDER_PIN) >> 3);
    }

    //-----add
}

void matrix_scan_user(void) {
    slider();
}