/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    mmu.cpp: Base MMU type
*/

#include <component/mmu/mmu.hpp>
#include <component/addrspace.hpp>

namespace Iris
{
    void ComponentMMU::Start()
    {
        AddrSpace::RegisterMMU(this);
    }

    void ComponentMMU::Shutdown()
    {
        // this is a default thing
        AddrSpace::RegisterMMU(nullptr);
    }
};