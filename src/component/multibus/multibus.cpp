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
        
        AddrSpaceMapping mappingPaging = AddrSpaceMapping();
        mappingPaging.startAddr = MULTIBUS_PAGING_START;
        mappingPaging.endAddr = MULTIBUS_PAGING_END;
        mappingPaging.component = this;

        AddrSpace::AddMapping(mappingPaging);

        // map ip2 segment 5 (multibus IO)

        AddrSpaceMapping mappingIo = AddrSpaceMapping();
        mappingIo.startAddr = MULTIBUS_IO_START;
        mappingIo.endAddr = MULTIBUS_IO_END;
        mappingIo.component = this; 

        AddrSpace::AddMapping(mappingIo);

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

        return ((addr >= lastSlotWritten->memStart && addr <= lastSlotWritten->memEnd) 
            || (addr >= lastSlotWritten->ioStart && addr <= lastSlotWritten->ioEnd));
    }

    bool Multibus::SetCachedReadMapping(size_t addr)
    {
        for (Multibus::SlotMapping& slot : slotMappings)
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

    bool Multibus::SetCachedWriteMapping(size_t addr)
    {
        for (Multibus::SlotMapping& slot : slotMappings)
        {
            if (!slot.active)
                continue;

            if ((addr >= slot.memStart
            && addr <= slot.memEnd) 
            || (addr >= slot.ioStart
            && addr <= slot.ioEnd))
            {
                lastSlotWritten = &slot;
                return true;
            }
        }

        // fail
        lastSlotWritten = nullptr;
        return false; 
    }

    // this simulates the action of the user inserting a slot into the Multibus backplane.
    bool Multibus::AddSlotMapping(SlotMapping& slot)
    {
        slot.active = true; 

        // multibus is 24 bit
        if (slot.memStart
        && slot.memEnd)
        {
            slot.memStart = (multibusMemoryEnd - 0x100000) + (slot.memStart & 0xFFFFF);
            slot.memEnd = (multibusMemoryEnd - 0x100000) + (slot.memEnd & 0xFFFFF);
        }

        if (!slot.memStart && !slot.memEnd && !slot.ioStart && !slot.ioEnd)
        {
            Logger::Log(MULTIBUS_LOG_PREFIX, "Multibus::AddSlotMapping: At least one of I/O and Memory address range must be set for a multibus mapping!",
            LogChannels::Error);
            return false;
        }

        Logger::Log(MULTIBUS_LOG_PREFIX, "Added new multibus slot:", LogChannels::Debug);

        if (slot.memStart || slot.memEnd)
            Logger::Log(MULTIBUS_LOG_PREFIX, std::format("Memory range is {:x}-{:x}", slot.memStart, slot.memEnd).c_str(), LogChannels::Debug);
        
        if (slot.ioStart || slot.ioEnd)
            Logger::Log(MULTIBUS_LOG_PREFIX, std::format("I/O range is {:x}-{:x}", slot.ioStart, slot.ioEnd).c_str(), LogChannels::Debug);

        Logger::Log(MULTIBUS_LOG_PREFIX, std::format("Slot is {}, component is {}", slot.id + 1, slot.component->GetName()).c_str(), LogChannels::Debug);

        slotMappings.push_back(slot);

        return true; 
    }   

    uint8_t Multibus::Read8(size_t addr) 
    {
        if (!UseCachedReadSlot(addr))
            SetCachedReadMapping(addr);

        // if no lastslot read, we don't have anytihng to read.

        if (lastSlotRead)
            return lastSlotRead->component->Read8(addr);

        if (addr < MULTIBUS_IO_START)
            return memory->Read8(TranslateAddress(addr)); 
        else if (addr >= MULTIBUS_IO_START && !lastSlotWritten)
        {
            Logger::Log(MULTIBUS_LOG_PREFIX, std::format("Multibus::Read8 - invalid I/O read from 0x{:x}", addr).c_str(), LogChannels::Warning);
        }
        return 0xFF;
    }

    uint16_t Multibus::Read16(size_t addr)
    {
        if (!UseCachedReadSlot(addr))
            SetCachedReadMapping(addr);

        // if no lastslot read, we don't have anytihng to read.

        if (lastSlotRead)
            return lastSlotRead->component->Read16(addr);

        if (addr < MULTIBUS_IO_START)
            return memory->Read16(TranslateAddress(addr)); 
        else if (addr >= MULTIBUS_IO_START && !lastSlotWritten)
            Logger::Log(MULTIBUS_LOG_PREFIX, std::format("Multibus::Read16 - invalid I/O read from 0x{:x}", addr).c_str(), LogChannels::Warning);
            
        return 0xFF;
    }

    uint32_t Multibus::Read32(size_t addr) 
    {
        if (!UseCachedReadSlot(addr))
            SetCachedReadMapping(addr);

        // if no lastslot read, we don't have anytihng to read.

        if (lastSlotRead)
            return lastSlotRead->component->Read32(addr);

        if (addr < MULTIBUS_IO_START)
            return memory->Read32(TranslateAddress(addr)); 
        else if (addr >= MULTIBUS_IO_START && !lastSlotWritten)
            Logger::Log(MULTIBUS_LOG_PREFIX, std::format("Multibus::Read32 - invalid I/O read from 0x{:x}", addr).c_str(), LogChannels::Warning);

        return 0xFF;
    }

    void Multibus::Write8(size_t addr, uint8_t value) 
    {
        if (!UseCachedWriteSlot(addr))
            SetCachedWriteMapping(addr);

        if (lastSlotWritten)
            lastSlotWritten->component->Write8(addr, value); 
             
        if (addr < MULTIBUS_IO_START)
            memory->Write8(TranslateAddress(addr), value); 
        else if (addr >= MULTIBUS_IO_START && !lastSlotWritten)
            Logger::Log(MULTIBUS_LOG_PREFIX, std::format("Multibus::Write8 - invalid I/O write of 0x{:x} to 0x{:x}", value, addr).c_str(), LogChannels::Warning);
    }

    void Multibus::Write16(size_t addr, uint16_t value)
    {
        if (addr >= MULTIBUS_PAGING_START
        && addr <= MULTIBUS_PAGING_END)
        {
            UpdatePTEntry(addr, value);
        }

        if (!UseCachedWriteSlot(addr))
            SetCachedWriteMapping(addr);

        if (lastSlotWritten)
            lastSlotWritten->component->Write16(addr, value); 
                           
        if (addr < MULTIBUS_IO_START)
            memory->Write16(TranslateAddress(addr), value); 
        else if (addr >= MULTIBUS_IO_START && !lastSlotWritten)
            Logger::Log(MULTIBUS_LOG_PREFIX, std::format("Multibus::Write16 - invalid I/O write of 0x{:x} to 0x{:x}", value, addr).c_str(), LogChannels::Warning);
    }
    
    void Multibus::Write32(size_t addr, uint32_t value)
    {
        if (!UseCachedWriteSlot(addr))
            SetCachedWriteMapping(addr);

        if (lastSlotWritten)
            lastSlotWritten->component->Write16(addr, value); 
                           
        if (addr < MULTIBUS_IO_START)
            memory->Write32(TranslateAddress(addr), value); 
        else if (addr >= MULTIBUS_IO_START && !lastSlotWritten)
            Logger::Log(MULTIBUS_LOG_PREFIX, std::format("Multibus::Write32 - invalid I/O write of 0x{:x} to 0x{:x}", value, addr).c_str(), LogChannels::Warning);
    }

    // Multibus Paging
    size_t Multibus::TranslateAddress(size_t addr)
    {
        uint16_t index = ((addr & 0xFFFFF) >> 12) & 0x1FFF;
        uint32_t realFinalAddr = (pageTable[index] << 12) + (addr & 0xFFF);
        return realFinalAddr;
    }

    void Multibus::UpdatePTEntry(size_t addr, uint16_t value)
    {
        uint16_t index = ((addr & 0xFFFFF) >> 12) & 0x1FFF;
     
        Logger::Log(MULTIBUS_LOG_PREFIX, std::format("Multibus page {:x} now points to physical page {:x}", index, value).c_str(), LogChannels::Debug);
    }
    
    void Multibus::Shutdown()
    {
        cpu = nullptr;
        lastSlotRead = lastSlotWritten = nullptr; 
        slotMappings.clear();
    }
}; 