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

        extensionDC4 = new CoherentExtensionDC4(this);
        Coherent::RegisterExtension(extensionDC4);
    }

    uint16_t DC4::OnRead16(size_t addr)
    {
        uint16_t ret = 0x00;
        switch (addr)
        {
            case DC4_REG_FLAGS:
                ret = flags;
                break; 
            default:
                Logger::Log(LOG_PREFIX_DC4, std::format("UNKNOWN DC4 Read16 0x{:x} from 0x{:x}", ret, addr).c_str(), LogChannels::Warning);
                break;
        }
        Logger::Log(LOG_PREFIX_DC4, std::format("DC4 Read16 0x{:x} from 0x{:x}", ret, addr).c_str(), LogChannels::Debug);
        
        return ret;
    }    

    void DC4::OnWrite16(size_t addr, uint16_t value)
    {
        uint16_t ret = 0x00;
        switch (addr)
        {
            case DC4_REG_FLAGS:
                flags = value;
                break; 
            case DC4_REG_COLOURMAP_START ... DC4_REG_COLOURMAP_END:
                UpdateColourmap(addr, value);
                break; 
            default:
                Logger::Log(LOG_PREFIX_DC4, std::format("UNKNOWN DC4 Write16 0x{:x} to 0x{:x}", value, addr).c_str(), LogChannels::Warning);
                break;
        }

        Logger::Log(LOG_PREFIX_DC4, std::format("DC4 Write16 0x{:x} to 0x{:x}", value, addr).c_str(), LogChannels::Debug);
    }

    void DC4::UpdateColourmap(size_t addr, uint16_t value)
    {
        // check mapping info
        // DC4: bottom 3 bits are mapping info for multimap mode
        // Flags = 0 = Singlemap mode. Otherwise it basically determines which one of 16 256 colour maps we write to.

        //0x20 is used 
        uint32_t index = 0;

        if (flags & DC4_FLAG_REG_ADDRMAP)
            index = (flags & 0x0F) << 8 | (addr - DC4_REG_COLOURMAP_START);    
        else
            index = (addr - DC4_REG_COLOURMAP_START); // this indicates that it is single map.

        colourMap[index] = value & 0xFF00;
        colourMap[index + 1] = value & 0x00FF;
    }

    void DC4::Shutdown()
    {
        delete extensionDC4;
    }
};