/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    multibus.hpp: The Intel Multibus bus.
    This is the bus that is used for everything on the early IRISes, and most things on the later IRIS 3000.

    Most likely it was inheited from the SUN design that SGI bought back in '82. PM1/PM2/IP1 boards are fully multibus, IP2 boards,
    except for the CPU, GPU and FPU (?) (thees use their own private bus.)
*/

#pragma once
#include <Motion.hpp>
#include <component/addrspace.hpp>
#include <component/component.hpp>

// also depends on the CPU
#include <component/cpu/cpu.hpp>

namespace Motion
{
    #define MULTIBUS_MEMORY_START           0x40000000
    #define MULTIBUS_MEMORY_END             0x40FFFFFF
    
    #define MULTIBUS_IO_START               0x50000000
    #define MULTIBUS_IO_END                 0x5000FFFF

    #define MULTIBUS_LOG_PREFIX             "Multibus"

    #define MULTIBUS_NUM_IRQ                0x8

    class Multibus : public Component
    {
    public: 
        void Start() override; 

        const char* GetName() override { return "Intel Multibus"; };

        /* 
            Defines a multibus backplane slot 
            Aparently multibus kind of sucks and basically it seems like it just provides a mechanism of firing the IRQs and protection.
        */
        class Slot
        {  
            uint32_t id; 
            uint32_t irq; 
        }; 

        // other stuff may be dependent on the multibus, so start it first
        bool IsEarlyStart() override { return true; };

    private:
        void FireMultibusIRQ(int32_t number);

        // THE CPU, so we can fire an irq
        ComponentCPU* cpu; 
    
    };
};