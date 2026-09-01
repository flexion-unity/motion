/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    gf2_ge.cpp: 14 custom geometry engines.

    Basically, you have 14 custom geometry engine chips (which are four 32-bit alus and a microcode store with a config register loaded to switch function) 
    * first one is a fifo which converts from ieee 754 to 20.8 fixed point ("geometry accelerator")
    * next 4 basically make up a 4x4 matrix multiplier, 
    * the next 6 are clipping (and also z-buffering)
    * next 2 are scaling, they scale 2 coordinates at once each
    * last one converts back out to ieee 754 and fifo's
     
    Also Z Buffering is used

    Note that we don't model all these chips but actually model them as a single chip which performs functions of all of them (FBC)

    on the ip2 board it's not on multibus, it's a private bus, segment 6 and also parts of MBIO, but you can treat it as a static address that never changes
*/

#include <component/multibus/multibus.hpp>
#include <component/memory.hpp>
#include <component/gpu/juniper/gf2/gf2_ge.hpp>

namespace Motion
{
    Cvar* fakeGF2; 

    void GF2GE::Start()
    {
        fakeGF2 = Cvar::Get("fakeGF2", "0");

        if (!fakeGF2->GetValue())
            return;

        Logger::Log(GF2_GE_LOG_PREFIX, "Initialising FAKE GF2 board for PROM graphical mode testing");

        multibus = Emulation::GetMachine()->FindComponentByType<Multibus>();

        // GF
        Multibus::SlotMapping mapping = Multibus::SlotMapping(this);
        mapping.ioStart = GF2FAKE_START;
        mapping.ioEnd = GF2FAKE_END;
        mapping.id = GF2_MULTIBUS_SLOT;

        multibus->AddSlotMapping(mapping);
        
        AddrSpaceMapping privMapping = AddrSpaceMapping();
        privMapping.startAddr = GF2_PRIVATE_BUS_START;
        privMapping.endAddr = GF2_PRIVATE_BUS_END;
        privMapping.component = this;

        AddrSpace::AddMapping(privMapping);
    }

    void GF2GE::Tick()
    {
        
    }
}