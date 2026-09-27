/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    gf2_fbc.cpp: FBC Orchestration

    Controls AM2903 (Data) & AM2910 (Control)
    Loads microcode into both

    SGI wrote the microcode in highly macro'd C which was compiled into their "mas" microcode assembler (which has been mostly ported to Linux by me). 
    The MAS Binary then converted the microcode into a binary file which was then uploaded. There's 4096 16-bit (4x"slices" for each am2903), controlled by an AM2910.

    Each state was started by _NS and ended by _ES
    
    MICROCODE FORMAT:
    17 "fields"

*/
#include <component/component.hpp>
#include <component/gpu/juniper/gf2/gf2.hpp>

namespace Motion
{
    void GF2::FBCStart()
    {
        CoherentEditor::Settings settings;
        settings.buf = (uint8_t*)ucode;
        settings.bufSize = GF2_FBC_UCODE_SLICES * GF2_FBC_UCODE_STATES;
        settings.name = "FBC Microcode Editor";
        
        fbcUcodeEditor = new CoherentEditor(this, settings);
        Coherent::RegisterExtension(fbcUcodeEditor);

        am2903.Start();
        am2910.Start();
    }

    uint16_t GF2::FBCRead16(size_t addr)
    {
        uint16_t value = 0x00;
        
        // TODO:
        if (addr >= GF2_FBC_DATA_START && addr <= GF2_FBC_DATA_END
        && fbcFlagsWritten == 0xFF
        && UcodeAccessIsEnabled())
        {
            uint16_t ucodeValue = ucode[GetCurrentUcodeState(addr)][GetCurrentUcodeSlice()];
            // the top slice of each state is 8b its wide
            value = (GetCurrentUcodeSlice() == 3) ? (ucodeValue & 0xFF) : ucodeValue;
        }    
        else
        {
            switch (addr)
            {
                case GF2_FBC_FLAGS:
                    // ensure that GL2 init knows we are alive
                    fbcFlagsRead |= GF2_FBC_FLAGS_READ_FBC_ACK;
                    fbcFlagsRead |= GF2_FBC_FLAGS_READ_BPC_ACK;
                    
                    value = fbcFlagsRead;
                    break;
                case GF2_FBC_DATA_START:
                    value = FBCExecuteAlternativeCommand(lastFbcAltCommand);
                    break; 
            }
        }

        Logger::Log(GF2_FBC_LOG_PREFIX, std::format("FBC Read16 0x{:x} from 0x{:x}", value, addr).c_str(), LogChannels::Debug);
        return value; 
    }

    // the "real " fbc command set is passed through from the GEs

    uint16_t GF2::FBCExecuteAlternativeCommand(uint16_t id)
    {
        uint16_t ret = 0x00;

        switch (id)
        {
            case GF2_FBC_ALTCMD_GET_MICRO_VERSION:
                ret = GF2_FBC_MICRO_VERSION;
                break; 
            case GF2_FBC_ALTCMD_GET_SCRATCH_SIZE:
                ret = GF2_FBC_SCRATCH_SIZE;
                break;
        }

        Logger::Log(GF2_FBC_LOG_PREFIX, std::format("FBC Execute Alternative Command #{:x} returning 0x{:x}", id, ret).c_str(), LogChannels::Debug);
        return ret; 
    }

    void GF2::FBCWrite16(size_t addr, uint16_t value)
    {
        // write to ucode
        if (addr >= GF2_FBC_DATA_START && addr <= GF2_FBC_DATA_END
        && fbcFlagsWritten == 0xFE
        && UcodeAccessIsEnabled())
        {
            uint16_t state = GetCurrentUcodeState(addr);
            uint16_t slice = GetCurrentUcodeSlice();
            ucode[state][slice] = value;
        }
        else
        {
            switch (addr)
            {
                case GF2_FBC_FLAGS:
                    fbcFlagsWritten = value;
                    break;
                // "Alternative" Codes
                case GF2_FBC_DATA_START:  // not in condition to write uCode
                    lastFbcAltCommand = value;
                    break; 
                // ON WRITE TO FBCDATA, INITIATE UCODE OPERATIONS!
                // THE UCODE MUST BE RUN ACCORDING TO THE COMMAND, WHICH WAS WRITTEN TO FBCDATA!

            }
        }
        
        
        Logger::Log(GF2_FBC_LOG_PREFIX, std::format("FBC Write16 0x{:x} to 0x{:x}", value, addr).c_str(), LogChannels::Debug);
    }

    void GF2::FBCTick()
    {
        
    }

    void GF2::FBCExecuteCommand()
    {
        
    }
    
    void GF2::BPCExecuteCommand()
    {

    }

}; 