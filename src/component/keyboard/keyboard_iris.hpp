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

    // Send to req config
    #define KEYBOARD_CONFIG_REQUEST     0x10

    #define KEYBOARD_TYPE_IRIS          0xAA

    #define KEYBOARD_TYPE_4D60          0x6E // *** TODO: 68K PROM Version 3.0.11 and later ONLY ***
    #define KEYBOARD_SUBTYPE_4D60_ISO   0x00 // *** TODO: 68K PROM Version 3.0.11 and later ONLY ***
    #define KEYBOARD_SUBTYPE_4D60_STD   0x01 // *** TODO: 68K PROM Version 3.0.11 and later ONLY ***

    #define KEYBOARD_DUART_LINE         0    // DUART0 Channel A

    class KeyboardIris : public ComponentKeyboard
    {
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

    public: 
        virtual const char* GetName() override { return "IRIS Keyboard"; }; 
        void OnEvent(Event& evt) override;
        void Shutdown() override;
    private: 
        DUART68681* duart; 
    };
}