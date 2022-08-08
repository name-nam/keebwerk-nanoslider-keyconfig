# keebwerk-nanoslider-keyconfig
このプログラムはkeebwerk公式のものを改変したものです。  
改変元->https://github.com/jesusvallejo/nanokeymaps/blob/master/keymap.c  
公式の手順ではなく独自の使い方をするため、その方のためのプログラムとなっています。  
つまり、独自のやり方に都合のいいように内部処理を変更してあるのでご了承ください。  

# 手順について
ここでは、プログラムについての解説をしています。  
全体的なやり方については、私のブログまたはYouTubeをご覧ください。  
ブログのほうが詳しく記載されていますが、動画も見てくれると嬉しいです。  
Blog->  
YouTube->  

# プログラムの解説
公式でも解説はありますが、日本語版として書かせていただきます。  
また、独自のやり方を紹介しているので、プログラムを書き換える必要はありません。   
というか独自のやり方紹介を参考にしながら使う方は書き換えないでください。  
ファームウェア書き込みに成功してから、好きなように煮るなり焼くなりしてください。  

## キーコンフィグレイヤー
以下のコードの部分に当たります。  
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
これは、キー割り当てのレイヤーです。
身近なもので例えると、ゲーミングキーボードやマウスのプロファイルです。  
ゲームごとにボタン割り当てを変えられるようにレイヤーごとにボタン割り当てを変えることができます。  

## ボリュームコントロールレイヤー
以下のコードの部分に当たります。  
```c:keymap
enum custom_keycodes { // In order to add more volume controls, add here a custom keycode, ex: VLC
    DEFAULT= SAFE_RANGE,SPOTIFY,DISCORD,CHROME,VLC
};
```
これは、ボリュームコントールのレイヤーのようなものです。  
nano.sliderについているスライダーで音量調整をする際に、プログラムごとに音量調整が可能になります。  
具体的にいうと、変数に16進数が代入されて、それを判別することでプログラムごとの動作を可能にしています。  
 


## キー割り当て
以下のコードの部分に当たります。  
```c:keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_DEF] = LAYOUT(
        MO(_TOOGLE),
        KC_F18, KC_F19, KC_F20,
        KC_F21, LCTL(LSFT(KC_M)), LCTL(LSFT(KC_D)), KC_F24
    ),
//------------------------------省略------------------------------//
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
```
これは、各キーにどのようなキーを割り当てるかの設定です。  
LAYOUT()の中の配置は、nano.sliderの配置と同じになっています。  
基本的にMO(_TOOGLE)は変更しないほうがいいです。  
その下に書くプログラムは、下記のサイトを参考にしてください。  
https://docs.qmk.fm/#/keycodes  

## レイヤーごとのMIDI信号の変数
以下のコードの部分に当たります。  
```c:keymap
bool process_record_user(uint16_t keycode, keyrecord_t *record) { 
    switch (keycode) {
        case DEFAULT:
            if (record->event.pressed) {
                midi2vol= 0x3E;
            } else {
            }
            break;
//------------------------------省略------------------------------//
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
これは、ボリュームコントロールレイヤーに対する動作です。  
そのレイヤーがDEFAULTに設定された場合、midi2vol変数に16進数が代入されます。  
この値は、MIDI信号として送られています。  

## MIDI信号送信
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
これは、スライダーの値、ボリュームコントロールレイヤーの状態等をMIDI信号として送信するプログラムです。  
公式のプログラムを独自に改変してあります。改変部分は別途まとめてあります。  

# 公式プログラムからの改変点
独自のやり方をする際、MIDI信号が約4msごとに送られていました。  
そのため、音量調整に関するOSDが常時表示されてしまうため、スライダーが動かされていない場合はデータを送信しないようにしました。  
また、スライダーを停止した状態でも値が±1変化することがあり、同様にOSDが常時表示されてしまうため、±1は誤差範囲としてスルーしています。  

# 問い合わせ
何か不明な点等ございましたら下記までご連絡ください(DMのみ受け付けます)。  
Twitter:@name__nam  
URL:https://twitter.com/name__nam  

# ライセンス
Copyright (c) 2022 name-nam  
このプログラムはGNU LGPLv3の下で配布しています。  
詳しくはライセンスをご覧ください。  
https://github.com/name-nam/keebwerk-nanoslider-keyconfig/blob/master/LICENSE