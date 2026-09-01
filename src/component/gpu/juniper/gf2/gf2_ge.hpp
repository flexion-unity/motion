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
    /// @brief sets up a fake GF2 which is enough to not cause a bus error. enough to get into PROM monitor in graphical mode for e.g. debugging UC4/DC4
    // THis makes no attempt to implement any functionality, just respond whe nteh bus accesses it
    // ONLY in Version 0.2.x!
    extern Cvar* fakeGF2; 

    #define GF2FAKE_START           0x50002000
    #define GF2FAKE_END             0x50002FFF

    #define GF2_PRIVATE_BUS_START   0x60000000
    #define GF2_PRIVATE_BUS_END     0x60001FFF

    #define GF2_MULTIBUS_SLOT       18

    #define GF2_GE_LOG_PREFIX       "GF2 - Geometry Engine"

    class GF2GE : public Component 
    {
    public: 
        const char* GetName() { return "Geometry Engine Rev 2.0/2.5"; }; 

        void Start() override;
        void Tick() override;
    private:
        GF2FBC* fbc; // needed for passthrough

        Multibus* multibus; 
    }; 
}; 