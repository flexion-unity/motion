/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    gf2_ucode.hpp: THe GF2 microcode. This is because I decided to write this DAMN thing in c++ and separate am2903/am2910 from rest of GF2 Board...POINTER HITLER

    The ucode is a shared resource, which is passed as a pointer or reference or something to all GF2-based resources
*/

#pragma once
#include <Motion.hpp>

namespace Motion
{
    // data is split into 4 slices.
    #define GF2_FBC_UCODE_STATES                    4096
    #define GF2_FBC_UCODE_SLICES                    4

    class GF2Ucode
    {
    public:             // this is against all principles of oop 
        uint16_t data[GF2_FBC_UCODE_STATES][GF2_FBC_UCODE_SLICES]; // 32kb 
        
    };
};