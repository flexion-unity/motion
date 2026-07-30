/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    keyboard.hpp: It's the keyboard.
    Generic keyboard interface

    On IRIS and early IRIS 4D, the keyboard I/O Interface is DUART0 Channel A. So we will hook to this. But this is the generic keyboard interface.#
    This class mostly exists so Machine::FindComponentByType can find all types of keyboards.
*/

#pragma once
#include <Motion.hpp>
#include <component/component.hpp>

namespace Motion
{
    class ComponentKeyboard : public Component
    {
        virtual const char* GetName() override { return "Keyboard Base Class (error)"; }; 
    };
}; 