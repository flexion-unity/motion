/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    dsd8217.cpp: The Qualogy (previously known as Data Systems Design) DSD 8217 Multibus Disk & Tape Controller
    
    Technically not used on the 3130 (3120) but this is the only controller that I've got a disk image for right now
    Later on we can run mkboot and boot this

    Currently this is a high-level emulation, but this uses the Intel 8085. Later on we'll write an 8085 emulation.
*/

#include <component/storage/dsd8217.hpp>

namespace Motion
{
    void DSD8217::Start()
    {
        multibus = Emulation::GetMachine()->FindComponentByType<Multibus>();
    }

    void DSD8217::Shutdown()
    {

    }
}; 