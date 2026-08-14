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
        // add the multibus state
        multibusExtension = new CoherentExtensionMultibus(this);
        Coherent::RegisterExtension(multibusExtension);

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

        // map the last megabyte of physical memory, so that multibus is the exclusive provider of writes to it
        // WARNING: Very KLUDGE! BAN THIS MAN FROM WRITING EMULATORS RIGHT NOW!
        // For example if System RAM is ever not at 0x0, it will explode.

        AddrSpaceMapping* memMapping = AddrSpace::GetMapping(0);
        
        // no physical memory
        if (!memMapping->endAddr)
        {
            Logger::Log(MULTIBUS_LOG_PREFIX, "No physical memory, multibus won't work anyway, skipping rest of init", LogChannels::Warning);
            return;
        }

        AddrSpaceMapping mappingMultibusMemory = AddrSpaceMapping();

        multibusMemoryStart = memMapping->endAddr - 0x100000;
        multibusMemoryEnd = memMapping->endAddr;
        mappingMultibusMemory.startAddr = multibusMemoryStart;
        mappingMultibusMemory.endAddr = multibusMemoryEnd;
        mappingMultibusMemory.component = this; 

        memMapping->endAddr = (memMapping->endAddr); // nuke 1 megabyte of system RAM

        AddrSpace::AddMapping(mappingMultibusMemory);

        // find the memory so we can use it
        if (!memory)
            memory = Emulation::GetMachine()->FindComponentByType<Memory>();

        // guaranteed, the CPU Initialises before this.
        if (!cpu)
            cpu = Emulation::GetMachine()->FindComponentByType<ComponentCPU>();
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

            if ((addr >= slot.memStart
            && addr <= slot.memEnd) 
            || (addr >= slot.ioStart
            && addr <= slot.ioEnd))
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

        // multibus is 24 bit
        if (slots[id].memStart
        && slots[id].memEnd)
        {
            slots[id].memStart = (multibusMemoryEnd - 0x100000) + (slots[id].memStart & 0xFFFFFF);
            slots[id].memEnd = (multibusMemoryEnd - 0x100000) + (slots[id].memEnd & 0xFFFFFF);
        }


        Logger::Log(MULTIBUS_LOG_PREFIX, std::format("Multibus slot {} now maps component {}, I/O range 0x{:x} to 0x{:x} (todo: impl memory)",
        id + 1, slot.component->GetName(), slot.ioStart, slot.ioEnd).c_str(), LogChannels::Debug);

        return true; 
    }   

    void Multibus::UpdateSlotIOMapping(int32_t id, size_t addrStart, size_t addrEnd)
    {
        if (id < 0 || id >= MULTIBUS_MAX_SLOTS)
        {
            Logger::Log(MULTIBUS_LOG_PREFIX, std::format("Multibus slot {} does not exist, expected 0-{}", id, MULTIBUS_MAX_SLOTS).c_str(), LogChannels::Warning);
            return; 
        }

        slots[id].ioStart = addrStart;
        slots[id].ioEnd = addrEnd;
    }

    void Multibus::UpdateSlotMemMapping(int32_t id, size_t addrStart, size_t addrEnd)
    {
        if (id < 0 || id >= MULTIBUS_MAX_SLOTS)
        {
            Logger::Log(MULTIBUS_LOG_PREFIX, std::format("Multibus slot {} does not exist, expected 0-{}", id, MULTIBUS_MAX_SLOTS).c_str(), LogChannels::Warning);
            return; 
        }

        // multibus is 24 bit
        slots[id].memStart = (multibusMemoryEnd - 0x100000) + (addrStart & 0xFFFFFF);
        slots[id].memEnd = (multibusMemoryEnd - 0x100000) + (addrEnd & 0xFFFFFF);
    }

    uint8_t Multibus::Read8(size_t addr) 
    {
        if (!UseCachedReadSlot(addr))
            if (!SetCachedReadSlot(addr))
            {
                // for MEMORY reads, if there is no Multibus device decoding this ram, we need to send them to the memory.
                // the switch register can disable multibus
                // for IO reads on IP2 (but not on PM2 ???) it's safe to do this

                if (addr >= multibusMemoryStart
                && addr <= multibusMemoryEnd)
                {
                    return memory->Read8(addr); 
                }
                else
                {
                    Logger::Log(MULTIBUS_LOG_PREFIX,
                    std::format("Multibus::Read8: SetCachedReadSlot FAILED: Unmapped Multibus read from 0x{:x}", addr).c_str(),
                    LogChannels::Warning);
                    return 0x00;
                }
            }

    returnMemory:
        return lastSlotRead->component->Read8(addr);
    }

    uint16_t Multibus::Read16(size_t addr)
    {
        if (!UseCachedReadSlot(addr))
            if (!SetCachedReadSlot(addr))
            {
                // for MEMORY reads, if there is no Multibus device decoding this ram, we need to send them to the memory.
                // the switch register can disable multibus
                // for IO reads on IP2 (but not on PM2 ???) it's safe to do this

                if (addr >= multibusMemoryStart
                && addr <= multibusMemoryEnd)
                {
                    return memory->Read16(addr); 
                }
                else
                {
                    Logger::Log(MULTIBUS_LOG_PREFIX,
                    std::format("Multibus::Read16: SetCachedReadSlot FAILED: Unmapped Multibus read from 0x{:x}", addr).c_str(),
                    LogChannels::Warning);
                    return 0x00;
                }
            }

        return lastSlotRead->component->Read16(addr);
    }

    uint32_t Multibus::Read32(size_t addr) 
    {
        if (!UseCachedReadSlot(addr))
            if (!SetCachedReadSlot(addr))
            {
                // for MEMORY reads, if there is no Multibus device decoding this ram, we need to send them to the memory.
                // the switch register can disable multibus
                // for IO reads on IP2 (but not on PM2 ???) it's safe to do this

                if (addr >= multibusMemoryStart
                && addr <= multibusMemoryEnd)
                {
                    return memory->Read32(addr); 
                }
                else
                {
                    Logger::Log(MULTIBUS_LOG_PREFIX,
                    std::format("Multibus::Read32: SetCachedReadSlot FAILED: Unmapped Multibus read from 0x{:x}", addr).c_str(),
                    LogChannels::Warning);
                    return 0x00;
                }
            }

        return lastSlotRead->component->Read32(addr);
    }

    void Multibus::Write8(size_t addr, uint8_t value) 
    {
        if (!UseCachedWriteSlot(addr))
            if (!SetCachedWriteSlot(addr))
            {
                // for MEMORY reads, if there is no Multibus device decoding this ram, we need to send them to the memory.
                // the switch register can disable multibus
                // for IO reads on IP2 (but not on PM2 ???) it's safe to do this

                if (addr >= multibusMemoryStart
                && addr <= multibusMemoryEnd)
                {
                    memory->Write8(addr, value); 
                }
                else
                {
                    Logger::Log(MULTIBUS_LOG_PREFIX,
                    std::format("Multibus::Write8: SetCachedWriteSlot FAILED: Unmapped Multibus write of 0x{:x} to 0x{:x}", value, addr).c_str(),
                    LogChannels::Warning);
                }
                return;
            }
            
        lastSlotRead->component->Write8(addr, value);
    }

    void Multibus::Write16(size_t addr, uint16_t value)
    {
        if (!UseCachedWriteSlot(addr))
            if (!SetCachedWriteSlot(addr))
            {
                // for MEMORY reads, if there is no Multibus device decoding this ram, we need to send them to the memory.
                // the switch register can disable multibus
                // for IO reads on IP2 (but not on PM2 ???) it's safe to do this

                if (addr >= multibusMemoryStart
                && addr <= multibusMemoryEnd)
                {
                    memory->Write16(addr, value); 
                }
                else
                {
                    Logger::Log(MULTIBUS_LOG_PREFIX,
                    std::format("Multibus::Write16: SetCachedWriteSlot FAILED: Unmapped Multibus write of 0x{:x} to 0x{:x}", value, addr).c_str(),
                    LogChannels::Warning);
                }
                return;
            }

        lastSlotRead->component->Write16(addr, value);
    }
    
    void Multibus::Write32(size_t addr, uint32_t value)
    {
        if (!UseCachedWriteSlot(addr))
            if (!SetCachedWriteSlot(addr))
            {
                // for MEMORY reads, if there is no Multibus device decoding this ram, we need to send them to the memory.
                // the switch register can disable multibus
                // for IO reads on IP2 (but not on PM2 ???) it's safe to do this

                if (addr >= multibusMemoryStart
                && addr <= multibusMemoryEnd)
                {
                    memory->Write32(addr, value); 
                }
                else
                {
                    Logger::Log(MULTIBUS_LOG_PREFIX,
                    std::format("Multibus::Write32: SetCachedWriteSlot FAILED: Unmapped Multibus write of 0x{:x} to 0x{:x}", value, addr).c_str(),
                    LogChannels::Warning);
                }
                return;
            }

        lastSlotRead->component->Write32(addr, value);       
    }

    void Multibus::Shutdown()
    {
        cpu = nullptr;
        lastSlotRead = lastSlotWritten = nullptr; 
        memset(slots, 0x00, sizeof(slots));
    }
}; 