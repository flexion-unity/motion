/*
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    dsd5217.cpp: The Qualogy (previously known as Data Systems Design) DSD 8217 Multibus Disk & Tape Controller

    Technically not used on the 3130 (3120) but this is the only controller that I've got a disk image for right now
    Later on we can run mkboot and boot this

    Currently this is a high-level emulation, but this uses the Intel 8085. Later on we'll write an 8085 emulation.

    NOTE: Due ot using an INTEL 8085, this is a LITTLE ENDIAN Peripheral. ALL I/O FROM THE IRIS IS BYTESWAPPED
    
    Sources:
    https://bitsavers.trailing-edge.com/pdf/dsd/5215_5217/040040-01_5215_Users_Guide_198404.pdf
    https://bitsavers.trailing-edge.com/pdf/dsd/5215_5217/040069-01_5217_Users_Guide_Addendu_198404.pdf
*/

#include <component/storage/dsd5217.hpp>

namespace Motion
{
    void DSD5217::Start()
    {
        multibus = Emulation::GetMachine()->FindComponentByType<Multibus>();

        Multibus::SlotMapping slot = Multibus::SlotMapping(this);
        slot.ioStart = DSD5217_MBIO_START;
        slot.ioEnd = DSD5217_MBIO_END;
        slot.id = DSD5217_MULTIBUS_SLOTNUM;

        // according to SGI this controller is "brain damaged" and requires hardcoded memory addresses
        slot.memStart = DSD5217_MEMORY_MAP1_START; // the first 16 bytes are used to determine memory size and we don'timplement the switch register yet
        slot.memEnd = DSD5217_MEMORY_MAP1_END;

        multibus->AddSlotMapping(slot);

        // ccb is start of chain of pointers
        ccbMapping = Multibus::SlotMapping(this);

        // bogus, will be overwritten
        ccbMapping.memStart = 0x1000;
        ccbMapping.memEnd = 0x10ff;
        ccbMapping.id = DSD5217_MULTIBUS_SLOTNUM;

        multibus->AddSlotMapping(ccbMapping);

        // open the hard drive
        hdd = Profile::OpenDisk(0);

        dsdExtension = new CoherentExtensionDSD5217(this);
        Coherent::RegisterExtension(dsdExtension);
    }

    uint8_t DSD5217::Read8(size_t addr)
    {
        if (!hdd)
            return 0xFF; // there is no point

        addr &= 0xFFFFF;

        uint8_t ret = 0x00;


        switch (addr)
        {
        case DSD5217_MBIO_STATUS:
            ret = state;
            break;
        default:
            if (wub.extension != DSD5217_24BIT_ADDRESSING)
            {
                Logger::Log(DSD5217_LOG_PREFIX, "DSD5217::Read8 - 20-bit segmented addressing is not implemented", LogChannels::Warning);
                return 0xFF;
            }

            // we have no idea where the buffer is in memory, or what its size is. so we have to ignore it if we want to read from some other register
            bool isBufIo = true;

            // structure read
            // seems lke addresses in 24 bit segment mode are silently ANDed with FFFFF0
            if (wub.ccbPtr && 
                addr >= (wub.ccbPtr & 0xFFFFF0) && (addr <= ((wub.ccbPtr & 0xFFFFF0) + sizeof(CCB))))
            {
                isBufIo = false;
                ret = *(((uint8_t *)&ccb) + (addr - (wub.ccbPtr & 0xFFFFF0)));
            }

            if (ccb.cibPtr 
                && addr >= (ccb.cibPtr & 0xFFFFF0) && (addr <= (ccb.cibPtr & 0xFFFFF0) + sizeof(CIB)))
            {
                isBufIo = false;
                ret = *(((uint8_t *)&cib) + (addr - (ccb.cibPtr & 0xFFFFF0)));
            }

            if (cib.iopbPtr 
                && addr >= (cib.iopbPtr & 0xFFFFF0) && (addr <= (cib.iopbPtr & 0xFFFFF0) + sizeof(IOPB)))
            {
                isBufIo = false;
                ret = *(((uint8_t *)&iopb) + (addr - (cib.iopbPtr & 0xFFFFF0)));
            }

            // some buffers are intneral to controller and others are in multibus ram...
            if (iopb.dba
            && isBufIo && addr >= iopb.dba)
                ret = ReadBuffer(addr);

            break; 
        }

        Logger::Log(DSD5217_LOG_PREFIX, std::format("DSD 5217 Read8 0x{:x} from 0x{:x}", ret, addr).c_str(), LogChannels::Debug);

        return ret;
    }

    void DSD5217::Write8(size_t addr, uint8_t value)
    {
        // don't bother if no hdd
        if (!hdd)
            return;

        addr &= 0xFFFFF;

        switch (addr)
        {
        case DSD5217_MBIO_STATUS:
            state = value;

            // if we are ready we are no longer busy
            ccb.busy = (state != DSD5217_MBIO_STATUS_IS_READY);

            if (state == DSD5217_MBIO_STATUS_IS_READY)
            {
                if (!initialStart)
                    ExecuteCommand();

                initialStart = false;
            }
            break;
        case DSD5217_WUB_EXTENSION:
            wub.extension = value;
            break; 
        case DSD5217_WUB_CCB_PTR:
            // start of our chain of ptrs.
            // technically a 32-bit pointer though
            wub.ccbPtr = (wub.ccbPtr & 0xFF00) | value;
            break;
        case DSD5217_WUB_CCB_PTR + 1:
            wub.ccbPtr = (wub.ccbPtr & 0x00FF) | (value << 8);

            // Now the CCB Pointer is specified. It's time to update our mapping.
            // Everything else is stored right after each other hopefulyl
            // It's a reference so we can do this.
            ccbMapping.memStart = wub.ccbPtr;
            ccbMapping.memEnd = wub.ccbPtr + 0xFF;

            break;
        // Non WUB
        default:

            // we have no idea where the buffer is in memory, or what its size is. so we have to ignore it if we want to read from some other register
            bool isBufIo = true;

            if (wub.extension != DSD5217_24BIT_ADDRESSING)
            {
                // causes too much log spam due to IDIOT sgi driver attempting to clear the registers of the DSD in 20-BIT SEGMENTED ADDRESSING MODE,
                // despite using 24-BIT LINEAR ADDRESSING with HARDCODED ADDRESSES and this being CLEARLY DOCUMENTED in the manual 
                //Logger::Log(DSD5217_LOG_PREFIX, "DSD5217::Write8 - 20-bit segmented addressing is not implemented", LogChannels::Warning);
                return;
            }

            // structure write
            if (wub.ccbPtr && 
                addr >= (wub.ccbPtr & 0xFFFFF0) && (addr <= ((wub.ccbPtr & 0xFFFFF0) + sizeof(CCB))))
            {
                isBufIo = false;
                *((uint8_t*)&ccb + (addr - wub.ccbPtr)) = value;
            }

            if (ccb.cibPtr 
                && addr >= (ccb.cibPtr & 0xFFFFF0) && (addr <= (ccb.cibPtr & 0xFFFFF0) + sizeof(CIB)))
            {
                isBufIo = false;
                *(((uint8_t*)&cib) + (addr - ccb.cibPtr)) = value;
            }

            if (cib.iopbPtr 
                && addr >= (cib.iopbPtr & 0xFFFFF0) && (addr <= (cib.iopbPtr & 0xFFFFF0) + sizeof(IOPB)))
            {
                isBufIo = false;
                *(((uint8_t*)&iopb) + (addr - cib.iopbPtr)) = value;
            }

            // some buffers are intneral to controller and others are in multibus ram...
            if (iopb.dba
            && isBufIo && addr >= iopb.dba)
                WriteBuffer(addr, value);

            break;
        }

        Logger::Log(DSD5217_LOG_PREFIX, std::format("DSD 5217 Write8 0x{:x} to 0x{:x}", value, addr).c_str(), LogChannels::Debug);
    }

    uint16_t DSD5217::Read16(size_t addr)
    {
        return (Read8(addr) | Read8(addr + 1) << 8);
    }

    void DSD5217::Write16(size_t addr, uint16_t value)
    {
        // Strangely enough, this is a little endian peripheral. Huh!
        Write8(addr, (value & 0x00FF));
        Write8(addr + 1, ((value & 0xFF00) >> 8));
    }

    //
    // command execution
    //
    void DSD5217::ExecuteCommand()
    {
        // we only emualte the hard drive right now
        if (!hdd)
            return; 

        ccb.busy = true;
        bool commandIsImplemented = true;
        size_t offset = 0;

        if (iopb.deviceCode != DSD5217_DEVICE_CODE_HDD)
        {
            Logger::Log(DSD5217_LOG_PREFIX, "Only HDD commands are currently supported! (QIC, Floppy not implemented!)", LogChannels::Warning);
            goto done; 
        }

        switch (iopb.function)
        {
            case DSD5217_FUNC_INIT:
                bufferType = DataBufferType::INIB;
                // read in the iIPB
                break;
            case DSD5217_FUNC_XFER_STATUS:
                bufferType = DataBufferType::ST;
                break;
            case DSD5217_FUNC_READ_DATA:
                ReadSector();
                break; 
            default:
                commandIsImplemented = false; 
                break;
        }

        if (!commandIsImplemented)
            Logger::Log(DSD5217_LOG_PREFIX, std::format("It's time to implement command 0x{:x}", iopb.function).c_str(), LogChannels::Debug);
        else
            Logger::Log(DSD5217_LOG_PREFIX, std::format("Executed command 0x{:x}", iopb.function).c_str(), LogChannels::Debug);

    done:
        cib.statusSemaphore = 0xFF;
        ccb.busy = false;

    }

    void DSD5217::ReadSector()
    {
        size_t offset = CHSToLinear();
        hdd->stream.seekp(offset, std::ios_base::beg);
        size_t bytesPerSector = inist.inib.bytesPerSectorHigh << 8 | inist.inib.bytesPerSectorLow;

        hdd->stream.read((char*)sectorBuffer, bytesPerSector);

        // VERY slow test code
        for (int32_t i = 0; i < bytesPerSector; i++)
            multibus->Write8(iopb.dba + i, sectorBuffer[i]);

        iopb.actualTransfers = bytesPerSector;
    }

    uint8_t DSD5217::ReadBuffer(int32_t offset)
    {  
        // already checked for valid dba above so don't bother

        uint8_t ret = 0xFF;

        switch (bufferType)
        {
            case DSD5217::DataBufferType::INIB:
                // INIB pointer
                if (offset >= (iopb.dba & 0xFFFFF0) && (offset < (iopb.dba & 0xFFFFF0) + sizeof(INIB)))
                {
                    ret = *(((uint8_t *)&inist.inib) + (offset - (iopb.dba & 0xFFFFF0)));
                }
                break;
            case DSD5217::DataBufferType::ST:
                // Status pointer
                // This is smaller in raw emulation mode of iSBC 215 (i.e. 20 bit mode)
                // But sgi doesn't use this (intentionally)
                if (offset >= (iopb.dba & 0xFFFFF0) && (offset < (iopb.dba & 0xFFFFF0) + DSD5217_SB_SIZE))
                {
                    ret = *(((uint8_t *)&inist.sb) + (offset - (iopb.dba & 0xFFFFF0)));
                }
                break; 
        }

        return ret;
    }

    void DSD5217::WriteBuffer(int32_t offset, uint8_t value)
    {
        switch (bufferType)
        {
            case DSD5217::DataBufferType::INIB:
                // INIB pointer
                if (offset >= (iopb.dba & 0xFFFFF0) && (offset < (iopb.dba & 0xFFFFF0) + sizeof(INIB)))
                {
                    *(((uint8_t*)&inist.inib) + (offset - iopb.dba)) = value;
                }
                break;
            case DSD5217::DataBufferType::ST:
                // Status pointer
                // This is smaller in raw emulation mode of iSBC 215 (i.e. 20 bit mode)
                // But sgi doesn't use this (intentionally)
                if (offset >= (iopb.dba & 0xFFFFF0) && (offset < (iopb.dba & 0xFFFFF0) + DSD5217_SB_SIZE))
                {
                    *(((uint8_t*)&inist.sb) + (offset - iopb.dba)) = value;
                }
        }
    }

    // Convert CHS to linear address
    // Probably should be in a "ComponentHDD" generic class
    size_t DSD5217::CHSToLinear()
    {
        size_t cylinderWeWant = iopb.cylinder;
        size_t headWeWant = iopb.head;
        size_t sectorWeWant = iopb.sector;

        // figure out the disk information
        size_t sectorsPerTrack = inist.inib.sectorsPerTrack;
        size_t bytesPerSector = inist.inib.bytesPerSectorHigh << 8 | inist.inib.bytesPerSectorLow;
        size_t numCyls = inist.inib.nrCylinders;

        size_t nrHeads = (iopb.deviceCode == DSD5217_DEVICE_CODE_FLOPPY) ? inist.inib.removableHeads : inist.inib.fixedHeads;
       
        // floppy - cyl's start at 1, otherwise 0
        if (iopb.deviceCode == DSD5217_DEVICE_CODE_FLOPPY)
            cylinderWeWant--;

        MOTION_ASSERT(cylinderWeWant >= numCyls, "****** INVALID DISK CYLINDER REQUEST!!! ******");

        size_t final = (((cylinderWeWant * nrHeads) + headWeWant) * (sectorWeWant)) * bytesPerSector;
        return final;
    }

    void DSD5217::AssertIRQLine()
    {
        multibus->FireMultibusIRQ(DSD5217_MULTIBUS_IRQ_LEVEL);
    }

    void DSD5217::Shutdown()
    {
        delete dsdExtension;
        Profile::Close(hdd);
    }
};