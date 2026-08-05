/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    memory.cpp: RAM 
*/

#include <base/emulation.hpp>
#include <component/memory.hpp>

namespace Motion 
{
    #define MEM_SEG0_START          0x0
    #define MEM_SEG1_START          0x1000000
    #define MEM_SEG2_START          0x2000000

  
    void Memory::Start()
    {
        auto capacity = Emulation::GetMachine().totalRamInstalled;
        ram = new uint8_t[capacity];
        Logger::Log(LOG_PREFIX_EMU_MACHINE, std::format("System RAM is {} bytes", capacity).c_str());
        
        AddrSpaceMapping mappingTd = AddrSpaceMapping();

        // gets mapped as text/data, stack and kernel

        // MMU segment 0 

        mappingTd.startAddr = 0x0;
        mappingTd.endAddr = mappingTd.startAddr + capacity;
        mappingTd.component = this;
        AddrSpace::AddMapping(mappingTd);

        // Temporary code 
        ram[0] = 0x33; // initial sp=0x33000800
        ram[2] = 0x08;
        ram[4] = 0x30; // initial pc=0x30000400 (otherwise you start executing the vector base)
        ram[6] = 0x04;
    }

    uint8_t Memory::OnRead8(size_t addr)
    {
        addr %= GetRamCapacity();

        if (addr >= GetRamCapacity())
        {
            Logger::Log("Memory::OnRead8 - Tried to read from invalid RAM address!");
            return 0;
        }   

        return ram[addr];
    }

    uint16_t Memory::OnRead16(size_t addr)
    {
        addr %= GetRamCapacity();

        if (addr >= GetRamCapacity())
        {
            Logger::Log("Memory::OnRead16 - Tried to read from invalid RAM address!");
            return 0;
        } 

        uint16_t* ram16 = (uint16_t*)ram;
        uint16_t value = ram16[addr >> 1];
        TOBE16(value);
        return value;
    }

    uint32_t Memory::OnRead32(size_t addr)
    {
        addr %= GetRamCapacity();

        if (addr >= GetRamCapacity())
        {
            Logger::Log("Memory::OnRead32 - Tried to read from invalid RAM address!");
            return 0;
        } 

        uint32_t* ram32 = (uint32_t*)ram;
        uint32_t value = ram32[addr >> 2];

        // IRIS is a big-endian system.
        TOBE32(value);

        return value;
    }

    void Memory::OnWrite8(size_t addr, uint8_t value)
    {
        addr %= GetRamCapacity();

        if (addr >= GetRamCapacity())
        {
            Logger::Log("Memory::OnWrite8 - Tried to write to invalid RAM address!");
            return;
        } 

        ram[addr] = value;
    }

    void Memory::OnWrite16(size_t addr, uint16_t value)
    {
        addr %= GetRamCapacity();

        if (addr >= GetRamCapacity())
        {
            Logger::Log("Memory::OnWrite16 - Tried to write to invalid RAM address!");
            return;
        } 
    
        uint16_t* ram16 = (uint16_t*)ram;       
        TOBE16(value);
        ram16[addr >> 1] = value;
    }

    void Memory::OnWrite32(size_t addr, uint32_t value)
    {
        addr %= GetRamCapacity();
        
        if (addr >= GetRamCapacity())
        {
            Logger::Log("Memory::OnWrite32 - Tried to write to invalid RAM address!");
            return;
        } 

        uint32_t* ram32 = (uint32_t*)ram;
        // IRIS is a big-endian system.
        TOBE32(value);
        ram32[addr >> 2] = value;

    }

    void Memory::Shutdown()
    {
        delete ram;
    }
}