/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    dc4.hpp: Silicon Graphics display controller version 4.

    This part of the graphics system receives input from the update controller UC4. It handles the actual presentation of the display
    as well as non-paletted graphics modes.
*/

#pragma once
#include <component/addrspace.hpp>
#include <component/component.hpp>
#include <component/multibus/multibus.hpp>

namespace Motion
{
    // Registers
    #define DC4_REG_START               0x50004000
    #define DC4_REG_END                 0x500047FF
    #define DC4_REG_FLAGS               0x50004000
    #define DC4_MULTIBUS_SLOT           17

    #define LOG_PREFIX_DC4              "DC4"

    #define DC4_FLAG_REG_ADDR0          (1 << 0)  // mapping
    #define DC4_FLAG_REG_ADDR1          (1 << 1)
    #define DC4_FLAG_REG_ADDR2          (1 << 2)
    #define DC4_FLAG_REG_ADDR3          (1 << 3)
    #define DC4_FLAG_BUS_ADDRMAP        (1 << 4)
    #define DC4_FLAG_REG_ADDRMAP        (1 << 5)
    #define DC4_FLAG_RGB_MODE           (1 << 6)  // bypass colour ram, use direct rgb colour
    #define DC4_FLAG_USE_UPPER_HALF     (1 << 7)  // dc4 use upper half of doubled double-width colour map 
    #define DC4_FLAG_OPTIONAL_CLOCK     (1 << 11)
    #define DC4_FLAG_PIPELINE_DEPTH_4   (1 << 12)
    #define DC4_FLAG_PROM               (1 << 13) // different prom
    #define DC4_FLAG_FREERUN            (1 << 14) // probably diag only

    #define DC4_REG_COLOURMAP_START     0x50004200
    #define DC4_REG_COLOURMAP_END       0x500047FF

    #define DC4_REG_COLOURMAP_RED       0x50004200
    #define DC4_REG_COLOURMAP_RED_END   0x500043FF
    #define DC4_REG_COLOURMAP_GREEN     0x50004400
    #define DC4_REG_COLOURMAP_GREEN_END 0x500045FF
    #define DC4_REG_COLOURMAP_BLUE      0x50004600
    #define DC4_REG_COLOURMAP_BLUE_END  0x500047FF

    #define DC4_COLOUR_RAM_SIZE         49152

    // Single map stuff
    #define DC4_SINGLEMAP_MASK          0xFFF

    // Multimap stuff
    #define DC4_MULTIMAP_NUM_MAPS       64
    #define DC4_MULTIMAP_MASK           0xFF

    class CoherentExtensionDC4 : public CoherentExtension
    {
    public:
        CoherentExtensionDC4(Component* owner) : CoherentExtension(owner) {}

        void AddUI() override;
    };

    class DC4 : public Component
    {
        friend class CoherentExtensionDC4;

    public: 
        void Start() override;
        void Shutdown() override;
        
        const char* GetName() override { return "GPU DC4 board (Display Controller v4)"; }; 

        // Register I/O
        // bus is 16 bit 
        uint16_t OnRead16(size_t addr) override;
        void OnWrite16(size_t addr, uint16_t value) override;

    private: 
        void UpdateColourmap(size_t addr, uint16_t value);

        uint8_t colourMap[DC4_COLOUR_RAM_SIZE]; // safe to put in BSS ????
        uint16_t flags; 
        CoherentExtensionDC4* extensionDC4; 
        Multibus* multibus;
    }; 
}; 