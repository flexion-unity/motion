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
    // data is split into 4 slices. since we only model four am2903s we don't bother with them. for now we can probably just figure out how to do it
    #define GF2_FBC_UCODE_STATES                    4096

    class GF2Ucode
    {
    public:             // this is against all principles of oop 
        uint16_t data[GF2_FBC_UCODE_STATES]; // 16kb 
        
    };
};