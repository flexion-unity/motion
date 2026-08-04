#include <component/gpu/juniper/dc4/dc4.hpp>

namespace Motion
{
    void DC4::Start()
    {
        // multibus is early start, guaranteed
        multibus = Emulation::GetMachine().FindComponentByType<Multibus>();

        Multibus::Slot slot = Multibus::Slot(this);

        slot.ioStart = DC4_REG_START;
        slot.ioEnd = DC4_REG_END;
        multibus->AddSlot(slot, DC4_MULTIBUS_SLOT);
    }

    void DC4::Shutdown()
    {

    }
};