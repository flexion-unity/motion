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

        uint8_t keyState;
        
        inline static Key keysSgiToAscii[KEYBOARD_NUM_KEYS] =
        {
    //       normal shift ctrl ctrlsh  alt  gs 
    //      SGI has an extra value here for no reason?
            { 0x80, 0x80, 0x80, 0x80, 0x80,  0 },	/* key  0(00) - BREAK KEY	*/
            { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  0 },	/* key  1(01) - SETUP KEY	*/
            { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  0 },	/* key  2(02) - LEFT CTRL KEY	*/
            { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  0 },	/* key  3(03) - CAPSLOCK KEY	*/
            { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  0 },	/* key  4(04) - RIGHTSHIFT KEY*/
            { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  0 },	/* key  5(05) - LEFTSHIFT KEY	*/
            { 0x1B, 0x1B, 0x1B, 0x1B, 0x1B,  0 },	/* key  6(06) - ESC KEY	*/
            { 0x31, 0x21, 0x31, 0x21, 0x31,  0 },	/* key  7(07) - ONE KEY	*/
            { 0x09, 0x09, 0x09, 0x09, 0x09,  0 },	/* key  8(08) - TAB KEY	*/
            { 0x71, 0x51, 0x11, 0x11, 0x40,  0 },	/* key  9(09) - Q KEY		*/
            { 0x61, 0x41, 0x01, 0x01, 0x61,  0 },	/* key 10(0A) - A KEY		*/
            { 0x73, 0x53, 0x13, 0x13, 0x73,  0 },	/* key 11(0B) - S KEY		*/
            { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  0 },	/* key 12(0C) - NOSCROLL KEY	*/
            { 0x32, 0x40, 0x32, 0x22, 0xFF,  1 },	/* key 13(0D) - TWO KEY	*/
            { 0x33, 0x23, 0x33, 0x23, 0xFF,  2 },	/* key 14(0E) - THREE KEY	*/
            { 0x77, 0x57, 0x17, 0x17, 0x77,  0 },	/* key 15(0F) - W KEY		*/
            { 0x65, 0x45, 0x05, 0x05, 0x65,  0 },	/* key 16(10) - E KEY		*/
            { 0x64, 0x44, 0x04, 0x04, 0x64,  0 },	/* key 17(11) - D KEY		*/
            { 0x66, 0x46, 0x06, 0x06, 0x66,  0 },	/* key 18(12) - F KEY		*/
            { 0x7A, 0x5A, 0x1A, 0x1A, 0xFF,  3 },	/* key 19(13) - Z KEY		*/
            { 0x78, 0x58, 0x18, 0x18, 0x78,  0 },	/* key 20(14) - X KEY		*/
            { 0x34, 0x24, 0x34, 0x24, 0x34,  0 },	/* key 21(15) - FOUR KEY	*/
            { 0x35, 0x25, 0x35, 0x25, 0x35,  0 },	/* key 22(16) - FIVE KEY	*/
            { 0x72, 0x52, 0x12, 0x12, 0x72,  0 },	/* key 23(17) - R KEY		*/
            { 0x74, 0x54, 0x14, 0x14, 0x74,  0 },	/* key 24(18) - T KEY		*/
            { 0x67, 0x47, 0x07, 0x07, 0x67,  0 },	/* key 25(19) - G KEY		*/
            { 0x68, 0x48, 0x08, 0x08, 0x68,  0 },	/* key 26(1A) - H KEY		*/
            { 0x63, 0x43, 0x03, 0x03, 0x63,  0 },	/* key 27(1B) - C KEY		*/
            { 0x76, 0x56, 0x16, 0x16, 0x76,  0 },	/* key 28(1C) - V KEY		*/
            { 0x36, 0x5E, 0x36, 0x5E, 0x36,  4 },	/* key 29(1D) - SIX KEY	*/
            { 0x37, 0x26, 0x37, 0x26, 0x37,  5 },	/* key 30(1E) - SEVEN KEY	*/
            { 0x79, 0x59, 0x19, 0x19, 0x79,  6 },	/* key 31(1F) - Y KEY		*/
            { 0x75, 0x55, 0x15, 0x15, 0x75,  0 },	/* key 32(20) - U KEY		*/
            { 0x6A, 0x4A, 0x0A, 0x0A, 0x6A,  0 },	/* key 33(21) - J KEY		*/
            { 0x6B, 0x4B, 0x0B, 0x0B, 0x6B,  0 },	/* key 34(22) - K KEY		*/
            { 0x62, 0x42, 0x02, 0x02, 0x62,  0 },	/* key 35(23) - B KEY		*/
            { 0x6E, 0x4E, 0x0E, 0x0E, 0x6E,  0 },	/* key 36(24) - N KEY		*/
            { 0x38, 0x2A, 0x38, 0x2A, 0x38,  7 },	/* key 37(25) - EIGHT KEY	*/
            { 0x39, 0x28, 0x39, 0x28, 0x39,  8 },	/* key 38(26) - NINE KEY	*/
            { 0x69, 0x49, 0x09, 0x09, 0x69,  0 },	/* key 39(27) - I KEY		*/
            { 0x6F, 0x4F, 0x0F, 0x0F, 0x6F,  0 },	/* key 40(28) - O KEY		*/
            { 0x6C, 0x4C, 0x0C, 0x0C, 0x6C,  0 },	/* key 41(29) - L KEY		*/
            { 0x3B, 0x3A, 0x3B, 0x3A, 0x3B,  9 },	/* key 42(2A) - SEMICOLON KEY	*/
            { 0x6D, 0x4D, 0x0D, 0x0D, 0xB5,  0 },	/* key 43(2B) - M KEY		*/
            { 0x2C, 0x3C, 0x2C, 0x3C, 0x2C, 10 },	/* key 44(2C) - COMMA KEY	*/
            { 0x30, 0x29, 0x30, 0x29, 0x30, 11 },	/* key 45(2D) - ZERO KEY	*/
            { 0x2D, 0x5F, 0x2D, 0x1F, 0x2D, 12 },	/* key 46(2E) - MINUS KEY	*/
            { 0x70, 0x50, 0x10, 0x10, 0x70,  0 },	/* key 47(2F) - P KEY		*/
            { 0x5B, 0x7B, 0x1B, 0x1B, 0x5B, 13 },	/* key 48(30) - LEFT BRACKET	*/
            { 0x27, 0x22, 0x27, 0x22, 0x27, 14 },	/* key 49(31) - QUOTE KEY	*/
            { 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 15 },	/* key 50(32) - RETURN KEY	*/
            { 0x2E, 0x3E, 0x2E, 0x3E, 0x2E, 16 },	/* key 51(33) - PERIOD KEY	*/
            { 0x2F, 0x3F, 0x2F, 0x3F, 0x2F, 17 },	/* key 52(34) - VIRGULE KEY	*/
            { 0x3D, 0x2B, 0x3D, 0x2B, 0x3D, 18 },	/* key 53(35) - EQUAL ACCENT 	*/
            { 0x60, 0x7E, 0x60, 0x7E, 0x60, 19 }, 	/* key 54(36) - ACCENTGRAVE KEY*/
            { 0x5D, 0x7D, 0x1D, 0x7D, 0x5D, 20 },	/* key 55(37) - RIGHTBRACKET KEY*/
            { 0x5C, 0x7C, 0x1C, 0x7C, 0x5C, 21 },	/* key 56(38) - BACKSLASH KEY	*/
            { 0x8A, 0x8A, 0x8A, 0x8A, 0x8A,  0 },	/* key 57(39) - PAD ONE KEY	*/
            { 0x89, 0x89, 0x89, 0x89, 0x89,  0 },	/* key 58(3A) - PAD ZERO KEY	*/
            { 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,  0 },	/* key 59(3B) - LINEFEED	*/
            { 0x08, 0x08, 0x08, 0x08, 0x08,  0 },	/* key 60(3C) - BACKSPACE KEY	*/
            { 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,  0 },	/* key 61(3D) - DELETE KEY	*/
            { 0x8D, 0x8D, 0x8D, 0x8D, 0x8D,  0 },	/* key 62(3E) - PAD FOUR KEY	*/
            { 0x8B, 0x8B, 0x8B, 0x8B, 0x8B,  0 },	/* key 63(3F) - PAD TWO KEY	*/
            { 0x8C, 0x8C, 0x8C, 0x8C, 0x8C,  0 },	/* key 64(40) - PAD THREE KEY	*/
            { 0x95, 0x95, 0x95, 0x95, 0x95,  0 },	/* key 65(41) - PAD PERIOD KEY*/
            { 0x90, 0x90, 0x90, 0x90, 0x90,  0 },	/* key 66(42) - PAD SEVEN KEY	*/
            { 0x91, 0x91, 0x91, 0x91, 0x91,  0 },	/* key 67(43) - PAD EIGHT KEY	*/
            { 0x8E, 0x8E, 0x8E, 0x8E, 0x8E,  0 },	/* key 68(44) - PAD FIVE KEY	*/
            { 0x8F, 0x8F, 0x8F, 0x8F, 0x8F,  0 },	/* key 69(45) - PAD SIX KEY	*/
            { 0x82, 0x82, 0x82, 0x82, 0x82,  0 },	/* key 70(46) - PAD PF2 KEY	*/
            { 0x81, 0x81, 0x81, 0x81, 0x81,  0 },	/* key 71(47) - PAD PF1 KEY	*/
            { 0x88, 0x88, 0x88, 0x88, 0x88,  0 },	/* key 72(48) - LEFTARROW KEY	*/
            { 0x86, 0x86, 0x86, 0x86, 0x86,  0 },	/* key 73(49) - DOWNARROW KEY	*/
            { 0x92, 0x92, 0x92, 0x92, 0x92,  0 },	/* key 74(4A) - PAD NINE KEY	*/
            { 0x93, 0x93, 0x93, 0x93, 0x93,  0 },	/* key 75(4B) - PAD MINUS KEY	*/
            { 0x94, 0x94, 0x94, 0x94, 0x94,  0 },	/* key 76(4C) - PAD COMMA KEY	*/
            { 0x84, 0x84, 0x84, 0x84, 0x84,  0 },	/* key 77(4D) - PAD PF4 KEY	*/
            { 0x83, 0x83, 0x83, 0x83, 0x83,  0 },	/* key 78(4E) - PAD PF3 KEY	*/
            { 0x87, 0x87, 0x87, 0x87, 0x87,  0 },	/* key 79(4F) - RIGHTARROW KEY*/
            { 0x85, 0x85, 0x85, 0x85, 0x85,  0 },	/* key 80(50) - UPARROW KEY	*/
            { 0x96, 0x96, 0x96, 0x96, 0x96,  0 },	/* key 81(51) - PAD ENTER KEY	*/
            { 0x20, 0x20, 0x20, 0x20, 0x20,  0 },	/* key 82(52) - SPACK KEY	*/
            { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  0 },	/* key 83(53) - LEFT  ALT KEY	*/
            { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  0 },	/* key 84(54) - RIGHT ALT KEY	*/
            { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  0 },	/* key 85(55) - RIGHT CTRL KEY*/
            { 0x81, 0x81, 0x81, 0x81, 0x81,  0 },	/* key 86(56) - F1->PF1	*/
            { 0x82, 0x82, 0x82, 0x82, 0x82,  0 },	/* key 87(57) - F2->PF2	*/
            { 0x83, 0x83, 0x83, 0x83, 0x83,  0 },	/* key 88(58) - F3->PF3	*/
            { 0x84, 0x84, 0x84, 0x84, 0x84,  0 },	/* key 89(59) - F4->PF4	*/
            { 0x00, 0x00, 0x00, 0x00, 0x00,  0 },	/* key 90(5A) - F5->SETUP	*/
            { 0x00, 0x00, 0x00, 0x00, 0x00,  0 },	/* key 91(5B) - F6		*/
            { 0x00, 0x00, 0x00, 0x00, 0x00,  0 },	/* key 92(5C) - F7		*/
            { 0x00, 0x00, 0x00, 0x00, 0x00,  0 },	/* key 93(5D) - F8		*/
            { 0x00, 0x00, 0x00, 0x00, 0x00,  0 },	/* key 94(5E) - F9		*/
            { 0x00, 0x00, 0x00, 0x00, 0x00,  0 },	/* key 95(5F) - F10		*/
            { 0x00, 0x00, 0x00, 0x00, 0x00,  0 },	/* key 96(60) - F11		*/
            { 0x00, 0x00, 0x00, 0x00, 0x00,  0 },	/* key 97(61) - F12		*/
            { 0x00, 0x00, 0x00, 0x00, 0x00,  0 },	/* key 98(62) - PRINT SCREEN	*/
            { 0x00, 0x00, 0x00, 0x00, 0x00,  0 },	/* key 99(63) - SCROLL LOCK	*/
            { 0x00, 0x00, 0x00, 0x00, 0x80,  0 },	/* key 100(64) - PAUSE/BREAK	*/
            { 0x97, 0x97, 0x97, 0x97, 0x97,  0 },	/* key 101(65) - INSERT	*/
            { 0x98, 0x98, 0x98, 0x98, 0x98,  0 },	/* key 102(66) - HOME		*/
            { 0x99, 0x99, 0x99, 0x99, 0x99,  0 },	/* key 103(67) - PAGE UP	*/
            { 0x9A, 0x9A, 0x9A, 0x9A, 0x9A,  0 },	/* key 104(68) - END		*/
            { 0x9B, 0x9B, 0x9B, 0x9B, 0x9B,  0 },	/* key 105(69) - PAGE DOWN	*/
            { 0x9C, 0x9C, 0x9C, 0x9C, 0x9C,  0 },	/* key 106(6A) - NUMBER LOCK	*/
            { 0xF7, 0xF7, 0xF7, 0xF7, 0xF7,  0 },	/* key 107(6B) - DIVISION KEY	*/
            { 0xD7, 0xD7, 0xD7, 0xD7, 0xD7,  0 },	/* key 108(6C) - MULTIPLY KEY	*/
            { 0x9D, 0x9D, 0x9D, 0x9D, 0x9D,  0 },	/* key 109(6D) - PAD PLUS KEY */
            { 0x3C, 0x3E, 0x3C, 0x3E, 0x3C,  0 },	/* key 110(6E) - LESS THAN KEY*/
        };
    };
}