/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    gf2_fbc.hpp: The Frame Buffer Controller, four AMD AM2903 chips running custom SGI Microcode.

    Also incorproates the Bitplane Controller, an interface to VRAM
*/

#pragma once
#include <component/component.hpp>

namespace Motion
{
    class GF2FBC : public Component 
    {
    public: 
        const char* GetName() { return "Framebuffer & Bitplane Controller (AMD Am2903)"; }; 
    }; 
}; 