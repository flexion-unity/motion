/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    uc4.hpp: Silicon Graphics update controller version 4.

    This part of the graphics system receives input from the GF2 (Graphics & Framebuffer 2) board and performs various 2D graphics actions such as stipple alpha,
    DDA-type lines and text (from a FONT rom). It uses a similar command interface to the FBC and GE on the GF2 board. It's required to perform all graphics actions.
    
    **** TODO ****
*/

#include <component/addrspace.hpp>
#include <component/component.hpp>
#include <component/multibus/multibus.hpp>

namespace Motion
{
    // Registers

    class UC4 : public Component
    {
    public: 
        void Start() override;
        void Shutdown() override;
        
        const char* GetName() override { return "GPU UC4 board (Update Controller v4)"; }; 

    private: 

    }; 
}; 