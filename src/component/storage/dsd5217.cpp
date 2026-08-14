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

        Multibus::Slot slot = Multibus::Slot(this);
        slot.ioStart = DSD5217_MBIO_START;
        slot.ioEnd = DSD5217_MBIO_END;

        multibus->AddSlot(slot, DSD5217_MULTIBUS_SLOTNUM);
    }


    uint8_t DSD5217::Read8(size_t addr) 
    {
        uint8_t ret = 0x00;

        switch (addr)
        {
            case DSD5217_IO_PTR1:
                ret = state;
                break;
        }

        Logger::Log(DSD5217_LOG_PREFIX, std::format("DSD 8217 Read8 {} from {}", ret, addr).c_str(), LogChannels::Debug);
        return ret;
    }

    void DSD5217::Write8(size_t addr, uint8_t value)
    {
        switch (addr)
        {
            case DSD5217_IO_PTR1:
                state = value;
                break;
        }

        Logger::Log(DSD5217_LOG_PREFIX, std::format("DSD 8217 Write8 {} to {}", value, addr).c_str(), LogChannels::Debug);
    }

    void DSD5217::Shutdown()
    {

    }
}; 