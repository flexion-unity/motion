/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    gf2_fbc.hpp: The Frame Buffer Controller, four AMD AM2903 chips running custom SGI Microcode.
*/

#include <component/component.hpp>

namespace Motion
{
    class GF2FBC : public Component 
    {
    public: 
        const char* GetName() { "AMD Am2903 Framebuffer Controller"; }; 
    }; 
}; 