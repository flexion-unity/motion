/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    multibus_component.hpp: The component for multibus slots.
    Provides a facility to allow for the swapping of byte lanes. Multibus A8-A15 is swapped to A0-A7 of our '020.

    This isn't used for I/O only MB stuff like UC4 and DC4.
*/

#include <base/emulation.hpp>
#include <base/machine/machine.hpp>
#include <component/component.hpp>
#include <component/multibus/multibus.hpp>

namespace Motion
{
    class MultibusSlotComponent : public Component 
    {
    public: 
        MultibusSlotComponent() : Component() { };

        void Start() override 
        {
            multibus = Emulation::GetMachine()->FindComponentByType<Multibus>();
        }; 

        uint8_t MBRead8(size_t mbAddr)
        {
            return multibus->ReadMB8(mbAddr ^ 1);
        }

        void MBWrite8(size_t mbAddr, uint8_t value)
        {
            multibus->WriteMB8(mbAddr ^ 1, value);
        }

        uint16_t MBRead16(size_t mbAddr)
        {
            return (uint16_t)(MBRead8(mbAddr) | (MBRead8(mbAddr + 1) << 8));
        }

        void MBWrite16(size_t mbAddr, uint16_t value)
        {
            MBWrite8(mbAddr, value & 0xFF);
            MBWrite8(mbAddr + 1, (value >> 8) & 0xFF);
        }

        uint32_t MBRead32(size_t mbAddr)
        {
            return (uint32_t)MBRead16(mbAddr) | ((uint32_t)MBRead16(mbAddr + 2) << 16);
        }

        void MBWrite32(size_t mbAddr, uint32_t value)
        {
            MBWrite16(mbAddr, value & 0xFFFF);
            MBWrite16(mbAddr + 2, (value >> 16) & 0xFFFF);
        }

    protected:
        /// @brief pointer to the multibus
        Multibus* multibus;

    };
}; 