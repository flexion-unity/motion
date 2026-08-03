/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    multibus.cpp: The Intel Multibus bus implementation.
*/

#include <base/emulation.hpp>
#include <component/multibus/multibus.hpp>

namespace Motion
{
    void Multibus::Start()
    {
        // map ip2 segment 4 (multibus memory)

        AddrSpaceMapping mappingMultibus = AddrSpaceMapping();
        mappingMultibus.startAddr = MULTIBUS_MEMORY_START;
        mappingMultibus.endAddr = MULTIBUS_MEMORY_END;
        mappingMultibus.component = this;

        AddrSpace::AddMapping(mappingMultibus);

        // map ip2 segment 5 (multibus IO)

        AddrSpaceMapping mappingIo = AddrSpaceMapping();
        mappingIo.startAddr = MULTIBUS_IO_START;
        mappingIo.endAddr = MULTIBUS_IO_END;
        mappingIo.component = this; 

        AddrSpace::AddMapping(mappingIo);
        
        // guaranteed, the CPU Initialises before this.
        if (!cpu)
            cpu = Emulation::GetMachine().FindComponentByType<ComponentCPU>();

        // add the multibus state
        multibusExtension = new CoherentExtensionMultibus(this);
        Coherent::RegisterExtension(multibusExtension);
    }

    void Multibus::FireMultibusIRQ(int32_t number)
    {
        if (number > MULTIBUS_NUM_IRQ)
        {
            Logger::Log(MULTIBUS_LOG_PREFIX, std::format("Tried to fire invalid IRQ #{}", number).c_str(), LogChannels::Warning);
            return;
        }

        cpu->SetIRQLine(number);
    }

    // is this stuff even faster 

    bool Multibus::UseCachedReadSlot(size_t addr)
    {
        if (!lastSlotRead)
            return false;

        return (addr >= lastSlotRead->ioStart
        && addr <= lastSlotRead->ioEnd);
    }

    bool Multibus::UseCachedWriteSlot(size_t addr)
    {
        if (!lastSlotWritten)
            return false;

        return (addr >= lastSlotWritten->ioStart
        && addr <= lastSlotWritten->ioEnd);
    }

    bool Multibus::SetCachedReadSlot(size_t addr)
    {
        for (Multibus::Slot& slot : slots)
        {
            if (!slot.active)
                continue;

            if (addr >= slot.ioStart
            && addr <= slot.ioEnd)
            {
                lastSlotRead = &slot;
                return true;
            }
        }

        // fail
        lastSlotRead = nullptr;
        return false; 
    }

    bool Multibus::SetCachedWriteSlot(size_t addr)
    {
        for (Multibus::Slot& slot : slots)
        {
            if (!slot.active)
                continue;

            if (addr >= slot.ioStart
            && addr <= slot.ioEnd)
            {
                lastSlotRead = &slot;
                return true;
            }
        }

        // fail
        lastSlotRead = nullptr;
        return false; 
    }

    // this simulates the action of the user inserting a slot into the Multibus backplane.
    bool Multibus::AddSlot(Slot slot, int32_t id)
    {
        if (id < 0 || id >= MULTIBUS_MAX_SLOTS)
        {
            Logger::Log(MULTIBUS_LOG_PREFIX, std::format("Multibus slot {} does not exist, expected 0-{}", id, MULTIBUS_MAX_SLOTS).c_str(), LogChannels::Warning);
            return false; 
        }

        if (slots[id].active)
        {
            Logger::Log(MULTIBUS_LOG_PREFIX, std::format("You already added that multibus slot {}", id).c_str(), LogChannels::Warning);
            return false; 
        }

        slots[id] = slot;
        slots[id].active = true;
        return true; 
    }

    uint8_t Multibus::OnRead8(size_t addr) 
    {
        if (!UseCachedReadSlot(addr))
            if (!SetCachedReadSlot(addr))
            {
                Logger::Log(MULTIBUS_LOG_PREFIX,
                std::format("Multibus::OnRead8: SetCachedReadSlot FAILED: Unmapped Multibus read from 0x{:x}", addr).c_str(),
                LogChannels::Warning);
            }

        return lastSlotRead->component->OnRead8(addr);
    }

    uint16_t Multibus::OnRead16(size_t addr)
    {
        if (!UseCachedReadSlot(addr))
            if (!SetCachedReadSlot(addr))
            {
                Logger::Log(MULTIBUS_LOG_PREFIX, 
                std::format("Multibus::OnRead16: SetCachedReadSlot FAILED: Unmapped Multibus read from 0x{:x}", addr).c_str(),
                LogChannels::Warning);
            }

        return lastSlotRead->component->OnRead16(addr);
    }

    uint32_t Multibus::OnRead32(size_t addr) 
    {
        if (!UseCachedReadSlot(addr))
            if (!SetCachedReadSlot(addr))
            {
                Logger::Log(MULTIBUS_LOG_PREFIX, 
                std::format("Multibus::OnRead32: SetCachedReadSlot FAILED: Unmapped Multibus read from 0x{:x}", addr).c_str(),
                LogChannels::Warning);
            }

        return lastSlotRead->component->OnRead32(addr);
    }

    void Multibus::OnWrite8(size_t addr, uint8_t value) 
    {
        if (!UseCachedWriteSlot(addr))
            if (!SetCachedWriteSlot(addr))
            {
                Logger::Log(MULTIBUS_LOG_PREFIX, 
                std::format("Multibus::OnWrite8: SetCachedWriteSlot FAILED: Unmapped Multibus write of 0x{:x} to 0x{:x}", value, addr).c_str(),
                LogChannels::Warning);
            }
            
        lastSlotRead->component->OnWrite8(addr, value);
    }

    void Multibus::OnWrite16(size_t addr, uint16_t value)
    {
        if (!UseCachedWriteSlot(addr))
            if (!SetCachedWriteSlot(addr))
            {
                Logger::Log(MULTIBUS_LOG_PREFIX, 
                std::format("Multibus::OnWrite16: SetCachedWriteSlot FAILED: Unmapped Multibus write of 0x{:x} to 0x{:x}", value, addr).c_str(),
                LogChannels::Warning);
            }

        lastSlotRead->component->OnWrite16(addr, value);
    }
    
    void Multibus::OnWrite32(size_t addr, uint32_t value)
    {
        if (!UseCachedWriteSlot(addr))
            if (!SetCachedWriteSlot(addr))
            {
                Logger::Log(MULTIBUS_LOG_PREFIX, 
                std::format("Multibus::OnWrite32: SetCachedWriteSlot FAILED: Unmapped Multibus write of 0x{:x} to 0x{:x}", value, addr).c_str(),
                LogChannels::Warning);
            }

        lastSlotRead->component->OnWrite32(addr, value);       
    }

    void Multibus::Shutdown()
    {
        cpu = nullptr;
        lastSlotRead = lastSlotWritten = nullptr; 
        memset(slots, 0x00, sizeof(slots));
    }
}; 