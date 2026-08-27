/*
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    ip2_mouse.hpp: A mouse

    TODO: Everything
    THis just decodes 30800000...31000000 in order to prevent horrible bus error
*/

#include <component/addrspace.hpp>
#include <component/component.hpp>

namespace Motion
{
    #define IP2_MOUSE_BUTTONS               0x30800000              // Buttons and revision
    #define IP2_MOUSE_QUADRATURE            0x31000000              // Quadrature gatheritng size

    #define IP2_MOUSE_MAPPING_START         IP2_MOUSE_BUTTONS
    #define IP2_MOUSE_MAPPING_END           IP2_MOUSE_QUADRATURE

    #define IP2_MOUSE_REVISION_B            0x10

    class IP2Mouse : public Component
    {
    public:
        void Start() override
        {
            AddrSpaceMapping mapping = AddrSpaceMapping();

            mapping.startAddr = IP2_MOUSE_MAPPING_START;
            mapping.endAddr = IP2_MOUSE_MAPPING_END;
            mapping.component = this;

            AddrSpace::AddMapping(mapping);
        }

        uint8_t Read8(size_t addr) override
        {
            uint8_t ret = 0x00;

            switch (addr)
            {
                case IP2_MOUSE_BUTTONS:
                    ret = IP2_MOUSE_REVISION_B; 
                    break;
            }
            
            return ret;
        }

        const char* GetName() override { return "IRIS Mouse (IP2)";};
    }; 
};