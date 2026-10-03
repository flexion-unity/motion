/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    am2903.hpp: The AMD Am2903 cascadable bitslice microcoded processor
    4 are used for the FBC
    
    In order to reduce complexity we basically model this strictly as four units rather than messing around with slice nonsense.

    Also, an AMD Am2910 Microcode Processor is 
    This is technically a problem but I don't think any other SGI systems use the AM2903.    
    Source: https://www.datasheets360.com/pdf/-6069213202016663880
*/

#pragma once
#include <component/component.hpp>
#include <component/gpu/juniper/gf2/gf2_ucode.hpp>

namespace Motion
{
    #define AM2903_LOG_PREFIX           "GF2 FBC Microcode Processor (AMD Am2903)"
    #define AM2903_INTERNAL_RAM_SIZE    16          // AM2903 Intenral ram size

    class AM2903
    {
    public: 
        AM2903(GF2Ucode* ucode)
        {
            this->ucode = ucode;
        }

        void Start();
        void ExecuteUcode(uint16_t ucode);
        void Tick(); 

    private: 
        uint16_t q;

        // latched on two ports
        uint16_t ram[AM2903_INTERNAL_RAM_SIZE] = {0}; 

        uint16_t ramAddrA, ramAddrB;
        uint16_t io;

        GF2Ucode* ucode; 

        bool running = false; 

    }; 
}; 