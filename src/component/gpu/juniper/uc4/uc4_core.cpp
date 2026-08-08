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
        multibus = Emulation::GetMachine()->FindComponentByType<Multibus>();

        Multibus::Slot slot = Multibus::Slot(this);

        slot.ioStart = UC4_REG_START;
        slot.ioEnd = UC4_REG_END;
        multibus->AddSlot(slot, UC4_MULTIBUS_SLOT);

        extensionUC4 = new CoherentExtensionUC4(this);
        Coherent::RegisterExtension(extensionUC4);
    }

    uint16_t UC4::Read16(size_t addr) 
    {
        uint16_t value = 0xFF;

        switch (addr)
        {
            case UC4_REG_UCR:
                value = ucr;
                break;
        }

        return 0xFF;
    }

    void UC4::Write16(size_t addr, uint16_t value)
    {
        switch (addr)
        {
            case UC4_REG_UCR:
                ucr = value;
                break; 
        }
    }

    void UC4::Shutdown()
    {
        
    }
};