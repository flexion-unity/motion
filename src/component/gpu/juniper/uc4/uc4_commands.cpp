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
        uint32_t fntAddr;

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
                        vram->Write32(vramAddr, colorcode & wecode);
                    }
                }
                break;
            case UC4_CMD_WRITEFONT:
                fmab &= UC4_FONT_ROM_SIZE - 1; 
                fontRom[fmab] = (value & 0xFF00);
                fontRom[fmab + 1] = (value >> 8) & 0xFF;
                fmab += 2; 
                break;
            case UC4_CMD_DRAWCHAR:

                // TODO: character at 0x40 is a stipple pattern
                for (int32_t y = ysb; y < yeb; y++)
                {
                    for (int32_t x = xsb; x < xeb; x++)
                    {
                        auto vramAddr = vram->GetVramAddress(x, y);
                        
                        // 1 byte per pixel
                        vram->Write32(vramAddr, fontRom[fmab]);

                        fntAddr++;
                        fntAddr &= UC4_FONT_ROM_SIZE - 1; 
                    }
                }
                break; 
        }

        // we are not busy anymore
        ucr &= (~UC4_UCR_BUSY);
    }
}