#include <component/addrspace.hpp>
#include <base/emulation.hpp>

namespace Motion
{
    // implements our cached path
    AddrSpaceMapping* AddrSpace::GetMapping(size_t addr) 
    { 
        // only check one mapping rather than iterating through all dozen mappings
        if (cachedMapping)
        {
            if (addr >= cachedMapping->startAddr
            && addr <= cachedMapping->endAddr)
                return cachedMapping;
        }

        // we are oging to have to optimise this    ``
        for (auto it : mappings)
        {
            if (addr >= it.second.startAddr
                && addr <= it.second.endAddr)
            {
                cachedMapping = &mappings[it.second.startAddr];
                // guaranteed to succeed since we *know* that the start address is *always* the key. if the impl changes we'll have to change this
                // yes, it could create an implicit kv pair if we don't
                return &mappings[it.second.startAddr];
            }
        }
        
        return nullptr; 
    }
 
    // non-cached path which doesn't break everything 
    AddrSpaceMapping* AddrSpace::PeekMapping(size_t addr) 
    { 
        // we are oging to have to optimise this    ``
        for (auto it : mappings)
        {
            if (addr >= it.second.startAddr
                && addr <= it.second.endAddr)
            {
                // guaranteed to succeed since we *know* that the start address is *always* the key. if the impl changes we'll have to change this
                // yes, it could create an implicit kv pair if we don't
                return &mappings[it.second.startAddr];
            }
        }
        
        return nullptr; 
    }

    bool AddrSpace::Translate(size_t addr, size_t* physAddr, bool isWrite)
    {
        if (mmu)
        {
            if (!mmu->Translate(addr, physAddr, isWrite))
            {
                SignalFault(addr, isWrite);
                return false;
            }
        }

        return true;
    }

    AddrSpaceMapping* AddrSpace::ReadCommon(size_t addr)
    {
        size_t physAddr = addr;

        if (!Translate(addr, &physAddr, false))
            return nullptr;

        AddrSpaceMapping* mapping = GetMapping(physAddr);
        return mapping;
    }

    AddrSpaceMapping* AddrSpace::PeekCommon(size_t addr)
    {
        size_t physAddr = addr;

        if (!Translate(addr, &physAddr, false))
            return nullptr;

        AddrSpaceMapping* mapping = PeekMapping(physAddr);
        return mapping;
    }

    AddrSpaceMapping* AddrSpace::WriteCommon(size_t addr)
    {
        size_t physAddr = addr;

        if (!Translate(addr, &physAddr, true))
            return nullptr;

        AddrSpaceMapping* mapping = PeekMapping(physAddr);
        return mapping;
    }

    void AddrSpace::LogUnmapped(const char* what, size_t addr, bool isWrite, uint32_t value)
    {
        Logger::Log(LOG_PREFIX_MAPPING, std::format("AddrSpace::{} - Unmapped {} of 0x{:x} to 0x{:x}!",
        (isWrite) ? "write" : "read", what, value, addr).c_str(), LogChannels::Warning);
    }

    uint8_t AddrSpace::ReadU8(size_t addr)
    {
        AddrSpaceMapping* mapping = ReadCommon(addr);

        if (mapping)
            return mapping->component->Read8(addr);
        else
        {
            BusError(addr, false, 8);
            LogUnmapped("ReadU8", addr, false, 8);
            return 0x00;
        }
    }
    
    uint16_t AddrSpace::ReadU16(size_t addr)
    {
        AddrSpaceMapping* mapping = ReadCommon(addr);

        if (mapping)
            return mapping->component->Read16(addr);
        else
        {
            BusError(addr, false, 16);
            LogUnmapped("ReadU16", addr, false, 16);
            return 0x00;
        }
    }
    
    uint32_t AddrSpace::ReadU32(size_t addr)
    {
        AddrSpaceMapping* mapping = ReadCommon(addr);

        if (mapping)
            return mapping->component->Read32(addr);
        else
        {
            BusError(addr, false, 32);
            LogUnmapped("ReadU32", addr, false, 32);
            return 0x00;
        }
    }
    
    int8_t AddrSpace::ReadS8(size_t addr) { return (int8_t)ReadU8(addr); };
    int16_t AddrSpace::ReadS16(size_t addr) { return (int16_t)ReadU16(addr); };
    int32_t AddrSpace::ReadS32(size_t addr) { return (int32_t)ReadU32(addr); };

    // todo: make readxx call this peakxx function

    uint8_t AddrSpace::PeekU8(size_t addr)
    {
        AddrSpaceMapping* mapping = PeekCommon(addr);

        if (mapping)
            return mapping->component->Read8(addr);
    
        return 0x00;
    }

    uint16_t AddrSpace::PeekU16(size_t addr)
    {
        AddrSpaceMapping* mapping = PeekCommon(addr);

        if (mapping)
            return mapping->component->Read16(addr);
    
        return 0x00;
    }

    uint32_t AddrSpace::PeekU32(size_t addr)
    {
        AddrSpaceMapping* mapping = PeekCommon(addr);

        if (mapping)
            return mapping->component->Read32(addr);
    
        return 0x00;
    }

    int8_t AddrSpace::PeekS8(size_t addr) { return (int8_t)PeekU8(addr); };
    int16_t AddrSpace::PeekS16(size_t addr) { return (int16_t)PeekU16(addr); };
    int32_t AddrSpace::PeekS32(size_t addr) { return (int32_t)PeekU32(addr); };

    void AddrSpace::AddMapping(AddrSpaceMapping mapping)
    {
        // this seems like a good place to put this
        // paranoid assumption of the mmu location
        // BAD CODE HACK

        if (mapping.startAddr > mapping.endAddr)
        {
            Logger::Log(LOG_PREFIX_MAPPING, "AddrSpace::AddMapping - mapping.StartAddr > mapping.endAddr", LogChannels::Error);
            return;
        }

        auto mappingCount = mappings.count(mapping.startAddr);

        if (mappingCount > 1)
        {
            Logger::Log(LOG_PREFIX_MAPPING, "AddrSpace::AddMapping - mapping already exists", LogChannels::Error);
            return;
        }

        if (!mapping.component)
        {
            Logger::Log(LOG_PREFIX_MAPPING, "AddrSpace::AddMapping - mapping doesn't have an attached component!", LogChannels::Error);
            return; 
        }

        Logger::Log(LOG_PREFIX_MAPPING, std::format("Added address mapping from 0x{:x} to 0x{:x} (size 0x{:x}) for component {}",
            mapping.startAddr, mapping.endAddr, (mapping.endAddr - mapping.startAddr), mapping.component->GetName()).c_str(), LogChannels::Debug);
        
        mappings[mapping.startAddr] = mapping;
    }

    void AddrSpace::WriteU8(size_t addr, uint8_t value)
    {
        AddrSpaceMapping* mapping = WriteCommon(addr);

        if (mapping)
            return mapping->component->Write8(addr, value);
        else
        {
            BusError(addr, true, 8);
            LogUnmapped("WriteU8", addr, true, value);
        }
    }

    void AddrSpace::WriteU16(size_t addr, uint16_t value)
    {
        AddrSpaceMapping* mapping = WriteCommon(addr);

        if (mapping)
            return mapping->component->Write16(addr, value);
        else
        {
            BusError(addr, true, 16);
            LogUnmapped("WriteU16", addr, true, value);
        }
    }

    void AddrSpace::WriteU32(size_t addr, uint32_t value)
    {
        AddrSpaceMapping* mapping = WriteCommon(addr);

        if (mapping)
            return mapping->component->Write32(addr, value);
        else
        {
            BusError(addr, true, 32);
            LogUnmapped("WriteU32", addr, true, value);
        }
    }

    void AddrSpace::WriteS8(size_t addr, int8_t value)
    {
        WriteU8(addr, (uint8_t)value);
    }

    void AddrSpace::WriteS16(size_t addr, int16_t value)
    {
        WriteU16(addr, (uint16_t)value);
    }

    void AddrSpace::WriteS32(size_t addr, int32_t value)
    {
        WriteU32(addr, (uint32_t)value);
    }

    // bus error stuff
    void AddrSpace::BusError(size_t addr, bool isWrite, size_t bits)
    {
        SignalFaultIfDeviceSpace(addr, isWrite);
        NotifyUnmapped(addr, isWrite, bits);
    }

    /*
        A hole in device space means nothing drove DSACK, the cycle times out and BERR is asserted.
        A read of memory that simply isn't fitted is NOT the same thing - it reads as zero, which is
        what MAME's IP2 RAM handler does and what the PROM's memory sizing loop depends on, since it
        walks down from 31MB writing patterns and reading them back to find the top of RAM.
    */
    void AddrSpace::SignalFaultIfDeviceSpace(size_t addr, bool isWrite)
    {
        if (addr < ADDRSPACE_DEVICE_SPACE_START)
            return;

        SignalFault(addr, isWrite);
    }

    void AddrSpace::SignalFault(size_t addr, bool isWrite)
    {
        if (!faultsEnabled)
            return;

        faultPending = true;
        faultAddress = addr;
        faultWasWrite = isWrite;
    }

    bool AddrSpace::TakeFault(size_t* addr, bool* isWrite)
    {
        if (!faultPending)
            return false;

        if (addr)
            *addr = faultAddress;

        if (isWrite)
            *isWrite = faultWasWrite;

        faultPending = false;
        return true;
    }

    void AddrSpace::RegisterMMU(ComponentMMU* mmu)
    {
        if (mmu)
            Logger::Log(LOG_PREFIX_MAPPING, std::format("Addressing system registered an MMU: {}", mmu->GetName()).c_str(), LogChannels::Debug);
        AddrSpace::mmu = mmu;
    }

    void AddrSpace::Shutdown()
    {
        // in case the bozo user forgot to actually shut down the mmu. it will be deleted anywya but then we will have a stale pointer.
        mmu = nullptr; 
        mappings.clear();
    }
}