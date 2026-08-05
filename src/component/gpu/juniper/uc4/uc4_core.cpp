/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    uc4_core.cpp: UC4 core stuff
*/

#include <component/gpu/juniper/uc4/uc4.hpp>

namespace Motion
{
    void UC4::Start()
    {
        // multibus is early start, guaranteed
        multibus = Emulation::GetMachine().FindComponentByType<Multibus>();

        Multibus::Slot slot = Multibus::Slot(this);

        slot.ioStart = UC4_REG_START;
        slot.ioEnd = UC4_REG_END;
        multibus->AddSlot(slot, UC4_MULTIBUS_SLOT);

    }

    void UC4::Shutdown()
    {
        
    }
};