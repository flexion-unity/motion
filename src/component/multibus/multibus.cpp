/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    multibus.cpp: The Intel Multibus bus implementation.
*/

#include <component/multibus/multibus.hpp>

namespace Motion
{
    void Multibus::Start()
    {
        // map ip2 segment 4 (multibus memory)

        AddrSpaceMapping mappingMultibus = AddrSpaceMapping();
        mappingMultibus.startAddr = MULTIBUS_MEMORY_START;
        mappingMultibus.endAddr = MULTIBUS_MEMORY_END;

        AddrSpace::AddMapping(mappingMultibus);

        AddrSpaceMapping mappingIo = AddrSpaceMapping();
        mappingIo.startAddr = MULTIBUS_IO_START;
        mappingIo.endAddr = MULTIBUS_IO_END;

        AddrSpace::AddMapping(mappingIo);
        
        // map ip2 segment 5 (multibus IO)
    }
}; 