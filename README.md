# keebwerk-nanoslider-keyconfig
[日本語版 README はこちら](https://github.com/name-nam/keebwerk-nanoslider-keyconfig/blob/master/README-ja.md)  
This program is a modified version of the official keebwerk one.  
Modified from->https://github.com/jesusvallejo/nanokeymaps/blob/master/keymap.c  
This program is designed for those who use their own way of doing things instead of the official procedure.  
In other words, please note that the internal process has been changed to suit your own unique way of doing things.  

# About the procedure
Here is an explanation of the program.  
For the overall procedure, please see my blog or YouTube.  
The blog describes it in more detail, but I hope you will watch the video as well.  
Blog->  
YouTube->  

# Explanation of the program
There is an official explanation of the program, but I will write it as a Japanese version.  
Also, there is no need to rewrite the program as it introduces our own way of doing it.   
Or rather, please do not rewrite the program if you use it while referring to the introduction of the original method.  
After you succeed in writing the firmware, please boil or bake it as you like.  

## Key Configuration Layer
This is the following piece of code.  
```c:keymaps
#define _DEF 0
#define _VOL 1
#define _MEDIA 2
#define _EXFC 3
#define _MFC 4
#define _EDIT 5
#define _RESET 6
#define _TOOGLE 7
```
This is a layer of key assignments.
A familiar example is the profile of a gaming keyboard or mouse.  
You can change the button assignments for each layer so that you can change the button assignments for each game.  

## Volume Control Layer
This is the following piece of code.  
```c:keymap
enum custom_keycodes { // In order to add more volume controls, add here a custom keycode, ex: VLC
    DEFAULT= SAFE_RANGE,SPOTIFY,DISCORD,CHROME,VLC
};
```
This is like a layer of volume control.  
When adjusting the volume with the slider attached to nano.slider, it is possible to adjust the volume for each program.  
Specifically, a hexadecimal number is assigned to the variable, and by determining it, per-program operation is made possible.  
 


## Key assignment
This is the part of the code below.  
```c:keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_DEF] = LAYOUT(
        MO(_TOOGLE),
        KC_F18, KC_F19, KC_F20,
        KC_F21, LCTL(LSFT(KC_M)), LCTL(LSFT(KC_D)), KC_F24
    ),
//------------------------------ omitted------------------------------//
    [_RESET] = LAYOUT(
        MO(_TOOGLE),
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, RESET 
    ),
    [_TOOGLE] = LAYOUT(
        MO(_TOOGLE),
        TO(_DEF), TO(_VOL), TO(_MEDIA),
        TO(_EXFC), TO(_MFC), TO(_EDIT), TO(_RESET)
    )
}
```
This sets what key to assign to each key.  
The arrangement in LAYOUT() is the same as in nano.slider.  
Basically, MO(_TOOGLE) should not be changed.  
Please refer to the following site for the program to be written under it.  
https://docs.qmk.fm/#/keycodes  

## MIDI signal variables per layer
This is the part of the code below.  
```c:keymap
bool process_record_user(uint16_t keycode, keyrecord_t *record) { 
    switch (keycode) {
        case DEFAULT:
            if (record->event.pressed) {
                midi2vol= 0x3E;
            } else {
            }
            break;
//------------------------------ omitted------------------------------//
        case VLC:
            if (record->event.pressed) {
                midi2vol= 0x42;
            } else {
            }
            break;
        
    }
    return true;
}
```
This is the behavior for the volume control layer.  
If that layer is set to DEFAULT, the midi2vol variable is assigned a hexadecimal value.  
This value is sent as a MIDI signal.  

## Send MIDI signal
```c:keymap
void slider(void) {
    if (divisor++) {
        return;
    }
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
}
```
This is a program to send slider values, volume control layer status, etc. as MIDI signals.  
This is an original modification of the official program. Modified parts are summarized separately.  

# Modifications from the official program
When doing things the original way, MIDI signals were sent about every 4ms.  
Because of this, the OSD related to volume adjustment is always displayed, so data is not sent when the slider is not being moved.  
Also, even when the slider is stopped, the value may change by +/-1, and the OSD is always displayed in the same way, so +/-1 is passed through as an error range.  

# Inquiries
If you have any questions, please contact us at the following address (only DM is accepted).  
Twitter:@name__nam  
URL:https://twitter.com/name__nam  

# License
Copyright (c) 2022 name-nam  
This program is distributed under GNU LGPLv3.  
Please see the license for details.  
https://github.com/name-nam/keebwerk-nanoslider-keyconfig/blob/master/LICENSE

# translation
This was translated by DeepL.
Original version is README-jp.