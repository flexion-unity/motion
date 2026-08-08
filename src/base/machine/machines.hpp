/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    machines.hpp: Header file that defines all machines.
*/

#pragma once
#include <base/machine/machine.hpp>

namespace Motion
{
    // this is extremely temporary
    extern Cvar* forceEnterSerialMonitor;

    /// @brief IRIS 3130 Machine
    class IRIS3130 : public Machine
    {
    public: 
        void AddComponents() override;
        const char* GetName() override { return "SGI IRIS 3130 (1986)"; };

        int32_t GetInternalScreenSizeX() override { return 1024; };
        int32_t GetInternalScreenSizeY() override { return 1024; };
    }; 
}; 