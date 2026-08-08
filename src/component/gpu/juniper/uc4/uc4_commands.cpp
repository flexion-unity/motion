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
        switch (commandId)
        {
            case UC4_CMD_SETCOLORAB:
                colorcode = value | (colorcode & 0xFFFF);
                break;
            case UC4_CMD_SETCOLORCD:
                colorcode = (colorcode << 16) | value;
                break;
            case UC4_CMD_SETWEAB:
                wecode = value | (wecode & 0xFFFF);
                break;
            case UC4_CMD_SETWECD:
                wecode = (wecode << 16) | value;
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
        }

        // we are not busy anymore
        ucr &= (~UC4_UCR_BUSY);
    }
}