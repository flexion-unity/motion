/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    uc4_core.cpp: UC4 command parsing engine
*/

#include <component/gpu/juniper/uc4/uc4.hpp>

namespace Motion
{
    // THis is a separate method because later we will have to inject commansd from the FBC's microcode on the GF2 board.
    void UC4::ParseCommand(size_t addr, uint16_t value)
    {
        ucr |= UC4_UCR_BUSY;

        // disabled 
        if (!(ucr & UC4_UCR_MBENAB))
            return; 

        uint32_t commandId = UC4_ADDR_TO_COMMAND(addr);
        uint32_t bufferNum = 0; 
        uint32_t fntAddr = fmab;

        switch (commandId)
        {
            case UC4_CMD_SETCOLORAB:
                colorcode = (colorcode & 0xFFFF0000) | (value & 0xFFFF);
                break;
            case UC4_CMD_SETCOLORCD:
                colorcode = (value << 16) | (colorcode & 0x0000FFFF);
                break;
            case UC4_CMD_SETWEAB:
                wecode = (wecode & 0xFFFF0000) | (value & 0xFFFF);
                break;
            case UC4_CMD_SETWECD:
                wecode = (value << 16) | (wecode & 0x0000FFFF);
                break;
            case UC4_CMD_FILLRECT:
                for (int32_t y = ysb; y < yeb; y++)
                {
                    for (int32_t x = xsb; x < xeb; x++)
                    {
                        auto vramAddr = vram->GetVramAddress(x, y);
                        vram->Write32(vramAddr, APPLY_WE_CODE(colorcode, vram->Read32(vramAddr), wecode));
                    }
                }
                break;
            case UC4_CMD_WRITEFONT:
                // why does this code work
                fontRom[fmab] = (value & 0xFF00) >> 8;
                fmab++;
                fmab &= UC4_FONT_ROM_SIZE - 1; 
                break;
            case UC4_CMD_DRAWCHAR:

                // TODO: character at 0x40 is a stipple pattern
                for (int32_t y = ysb; y < yeb; y++)
                {
                    uint8_t row = fontRom[fntAddr];
                    
                    for (int32_t x = xsb; x < xeb; x++)
                    {
                        bool bit = (row >> (7 - (x & 7))) & 1;
                        auto vramAddr = vram->GetVramAddress(x, y);

                        // 1 byte per pixel
                        vram->Write32(vramAddr, APPLY_WE_CODE((bit) ? colorcode : 0, vram->Read32(vramAddr), wecode)); // use a temp, as gl2 loads the fmab value before each char. so i assume it is not autoadvancing
                    }

                    fntAddr++;
                    fntAddr &= UC4_FONT_ROM_SIZE - 1; 
                }
                break; 
        }

        // we are not busy anymore
        ucr &= (~UC4_UCR_BUSY);
    }
}