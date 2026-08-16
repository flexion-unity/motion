/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    dsd5217.cpp: The Qualogy (previously known as Data Systems Design) DSD 8217 Multibus Disk & Tape Controller
    
    Technically not used on the 3130 (3120) but this is the only controller that I've got a disk image for right now
    Later on we can run mkboot and boot this

    Currently this is a high-level emulation, but this uses the Intel 8085. Later on we'll write an 8085 emulation.
*/

#include <component/storage/dsd5217.hpp>

namespace Motion
{
    void DSD5217::Start()
    {
        multibus = Emulation::GetMachine()->FindComponentByType<Multibus>();

        Multibus::SlotMapping slot = Multibus::SlotMapping(this);
        slot.ioStart = DSD5217_MBIO_START;
        slot.ioEnd = DSD5217_MBIO_END;
        slot.id = DSD5217_MULTIBUS_SLOTNUM;

        // according to SGI this controller is "brain damaged" and requires hardcoded memory addresses
        slot.memStart = DSD5217_MEMORY_MAP1_START; // the first 16 bytes are used to determine memory size and we don'timplement the switch register yet
        slot.memEnd = DSD5217_MEMORY_MAP1_END;

        multibus->AddSlotMapping(slot);

        hdd = Profile::OpenDisk(0);
    }


    uint8_t DSD5217::Read8(size_t addr) 
    {
        if (!hdd)
            return 0xFF; // there is no point

        addr &= 0xFFFFF;
        
        uint8_t ret = 0x00;

        switch (addr)
        {
            case DSD5217_MBIO_PTR1:
                ret = state;
                break;
        }
        
        Logger::Log(DSD5217_LOG_PREFIX, std::format("DSD 5217 Read8 0x{:x} from 0x{:x}", ret, addr).c_str(), LogChannels::Debug);
        
        return ret;
    }

    void DSD5217::Write8(size_t addr, uint8_t value)
    {
        // don't bother if no hdd
        if (!hdd)
            return;

        addr &= 0xFFFFF;
        
        switch (addr)
        {
            case DSD5217_MBIO_PTR1:
                state = value;
                break;
        }

        Logger::Log(DSD5217_LOG_PREFIX, std::format("DSD 5217 Write8 0x{:x} to 0x{:x}", value, addr).c_str(), LogChannels::Debug);
    }

    void DSD5217::Shutdown()
    {
        Profile::Close(hdd);
    }
}; 