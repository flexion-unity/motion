/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    gf2_fbc.hpp: The Frame Buffer Controller, four AMD AM2903 chips running custom SGI Microcode.
*/

#pragma once
#include <component/component.hpp>
#include <component/gpu/juniper/gf2/gf2_fbc.hpp>

namespace Motion
{
    class GF2GE : public Component 
    {
    public: 
        const char* GetName() { "Geometry Engine Rev 2.0/2.5"; }; 

        void Start() override;
        void Tick() override;
    private:
        GF2FBC* fbc; // needed for passthrough
    }; 
}; 