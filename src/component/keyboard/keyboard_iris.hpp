/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    keyboard_iris.hpp: It's the keyboard.
    Generic keyboard interface

    On IRIS and early IRIS 4D, the keyboard I/O Interface is DUART0 Channel A. So we will hook to this. But this is the generic keyboard interface.#
    This class mostly exists so Machine::FindComponentByType can find all types of keyboards.
*/

#pragma once
#include <base/event/event.hpp>
#include <component/keyboard/keyboard.hpp>
// since this is DUART A Channel 0 we can basically do this by injecting data into that channel
#include <component/ip2/ip2_duart.hpp> 

namespace Motion
{
    // this is here because the presence of the keyboard is how the serial monitor gets entered into.

    // Sent to req config
    #define KEYBOARD_CONFIG_REQUEST     0x10

    #define KEYBOARD_TYPE_IRIS          0xAA

    #define KEYBOARD_TYPE_4D60          0x6E        // *** TODO: 68K PROM Version 3.0.11 and later ONLY ***
    #define KEYBOARD_SUBTYPE_4D60_ISO   0x00        // *** TODO: 68K PROM Version 3.0.11 and later ONLY ***
    #define KEYBOARD_SUBTYPE_4D60_STD   0x01        // *** TODO: 68K PROM Version 3.0.11 and later ONLY ***

    #define KEYBOARD_DUART_LINE         0           // DUART0 Channel A
    #define KEYBOARD_NUM_KEYS           111

    // Modifers
    #define KEYBOARD_STATE_LEFT_SHIFT   (1 << 0)    
    #define KEYBOARD_STATE_RIGHT_SHIFT  (1 << 1)
    #define KEYBOARD_STATE_LEFT_CTRL    (1 << 2)
    #define KEYBOARD_STATE_RIGHT_CTRL   (1 << 3)
    #define KEYBOARD_STATE_LEFT_ALT     (1 << 4)
    #define KEYBOARD_STATE_RIGHT_ALT    (1 << 5)
    #define KEYBOARD_STATE_CAPSLOCK     (1 << 6)
    #define KEYBOARD_STATE_SETUP        (1 << 7)

    class KeyboardIris : public ComponentKeyboard
    {
    public: 
        // This is from GL2
        struct Key
        {
            uint8_t normal;                 // normal
            uint8_t shift;                  // shift
            uint8_t control;                // control
            uint8_t controlShift;           // ctrl+shifted ascii index
            uint8_t alt;                    // alt index
            uint8_t gs;                     // no idea what this is
        };

        virtual const char* GetName() override { return "IRIS Keyboard"; }; 
        void OnEvent(Event& evt) override;
        void Shutdown() override;
    private: 
        DUART68681* duart;

        bool initialised = false;

        // shut up the log files
        bool shutUpBeep = false; 
        bool shutUpLed = false; 

        // bit 7 is set for shift
        bool shiftStateL = false, shiftStateR = false;
        bool ctrlStateL = false, ctrlStateR = false; 

        // TODO: PF1-PF4, SETUP etc

        // Map physical key position (SDL_Scancode) to SGI keyboard device codes
        inline static std::unordered_map<SDL_Scancode, uint8_t> sdlToSgi =
        {
            { SDL_SCANCODE_ESCAPE,          6 },
            { SDL_SCANCODE_1,               7 },
            { SDL_SCANCODE_2,              13 },
            { SDL_SCANCODE_3,              14 },
            { SDL_SCANCODE_4,              21 },
            { SDL_SCANCODE_5,              22 },
            { SDL_SCANCODE_6,              29 },
            { SDL_SCANCODE_7,              30 },
            { SDL_SCANCODE_8,              37 },
            { SDL_SCANCODE_9,              38 },
            { SDL_SCANCODE_0,              45 },
            { SDL_SCANCODE_Q,               9 },
            { SDL_SCANCODE_A,              10 },
            { SDL_SCANCODE_S,              11 },
            { SDL_SCANCODE_W,              15 },
            { SDL_SCANCODE_E,              16 },
            { SDL_SCANCODE_D,              17 },
            { SDL_SCANCODE_F,              18 },
            { SDL_SCANCODE_Z,              19 },
            { SDL_SCANCODE_X,              20 },
            { SDL_SCANCODE_R,              23 },
            { SDL_SCANCODE_T,              24 },
            { SDL_SCANCODE_G,              25 },
            { SDL_SCANCODE_H,              26 },
            { SDL_SCANCODE_C,              27 },
            { SDL_SCANCODE_V,              28 },
            { SDL_SCANCODE_Y,              31 },
            { SDL_SCANCODE_U,              32 },
            { SDL_SCANCODE_J,              33 },
            { SDL_SCANCODE_K,              34 },
            { SDL_SCANCODE_B,              35 },
            { SDL_SCANCODE_N,              36 },
            { SDL_SCANCODE_I,              39 },
            { SDL_SCANCODE_O,              40 },
            { SDL_SCANCODE_L,              41 },
            { SDL_SCANCODE_M,              43 },
            { SDL_SCANCODE_P,              47 },
            { SDL_SCANCODE_SEMICOLON,      42 },
            { SDL_SCANCODE_COMMA,          44 },
            { SDL_SCANCODE_MINUS,          46 },
            { SDL_SCANCODE_LEFTBRACKET,    48 },
            { SDL_SCANCODE_APOSTROPHE,     49 },
            { SDL_SCANCODE_RETURN,         50 },
            { SDL_SCANCODE_PERIOD,         51 },
            { SDL_SCANCODE_SLASH,          52 },
            { SDL_SCANCODE_EQUALS,         53 },
            { SDL_SCANCODE_GRAVE,          54 },
            { SDL_SCANCODE_RIGHTBRACKET,   55 },
            { SDL_SCANCODE_BACKSLASH,      56 },
            { SDL_SCANCODE_KP_1,           57 },
            { SDL_SCANCODE_KP_0,           58 },
            { SDL_SCANCODE_KP_ENTER,       81 },
            { SDL_SCANCODE_KP_4,           62 },
            { SDL_SCANCODE_KP_2,           63 },
            { SDL_SCANCODE_KP_3,           64 },
            { SDL_SCANCODE_KP_PERIOD,      65 },
            { SDL_SCANCODE_KP_7,           66 },
            { SDL_SCANCODE_KP_8,           67 },
            { SDL_SCANCODE_KP_5,           68 },
            { SDL_SCANCODE_KP_6,           69 },
            { SDL_SCANCODE_KP_9,           74 },
            { SDL_SCANCODE_KP_MINUS,       75 },
            { SDL_SCANCODE_KP_COMMA,       76 },
            { SDL_SCANCODE_TAB,             8 },
            { SDL_SCANCODE_BACKSPACE,      60 },
            { SDL_SCANCODE_DELETE,         61 },
            { SDL_SCANCODE_SPACE,          82 },

            { SDL_SCANCODE_LEFT,           72 },
            { SDL_SCANCODE_DOWN,           73 },
            { SDL_SCANCODE_RIGHT,          79 },
            { SDL_SCANCODE_UP,             80 },

            { SDL_SCANCODE_LCTRL,           2 },
            { SDL_SCANCODE_CAPSLOCK,        3 },
            { SDL_SCANCODE_RSHIFT,          4 },
            { SDL_SCANCODE_LSHIFT,          5 },

            { SDL_SCANCODE_LALT,           83 },
            { SDL_SCANCODE_RALT,           84 },
            { SDL_SCANCODE_RCTRL,          85 },

            { SDL_SCANCODE_F1,             86 },
            { SDL_SCANCODE_F2,             87 },
            { SDL_SCANCODE_F3,             88 },
            { SDL_SCANCODE_F4,             89 },
            { SDL_SCANCODE_F5,             90 },
            { SDL_SCANCODE_F6,             91 },
            { SDL_SCANCODE_F7,             92 },
            { SDL_SCANCODE_F8,             93 },
            { SDL_SCANCODE_F9,             94 },
            { SDL_SCANCODE_F10,            95 },
            { SDL_SCANCODE_F11,            96 },
            { SDL_SCANCODE_F12,            97 },

            { SDL_SCANCODE_PRINTSCREEN,    98 },
            { SDL_SCANCODE_SCROLLLOCK,     99 },
            { SDL_SCANCODE_PAUSE,         100 },

            { SDL_SCANCODE_INSERT,        101 },
            { SDL_SCANCODE_HOME,          102 },
            { SDL_SCANCODE_PAGEUP,        103 },
            { SDL_SCANCODE_END,           104 },
            { SDL_SCANCODE_PAGEDOWN,      105 },

            { SDL_SCANCODE_NUMLOCKCLEAR,  106 },

            { SDL_SCANCODE_KP_DIVIDE,     107 },
            { SDL_SCANCODE_KP_MULTIPLY,   108 },
            { SDL_SCANCODE_KP_PLUS,       109 },

            { SDL_SCANCODE_NONUSBACKSLASH,110 },
        };
    };
}