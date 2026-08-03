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
    // we use a raw array because its the fastest and a lot of this stuff is EXTREMELY Hot path! Like UC4/DC4. 
    #define MULTIBUS_MAX_SLOTS              20  

    class Multibus : public Component
    {
    public: 
        void Start() override; 
        void Shutdown() override;

        const char* GetName() override { return "Intel Multibus"; };

        /* 
            Defines a multibus backplane slot 
            Aparently multibus kind of sucks and basically it seems like it just provides a mechanism of firing the IRQs and protection.
        */
        class Slot
        {  
            friend class Multibus; 

        public: 
            uint32_t irq; 
            size_t addrStart;
            size_t addrEnd; 
            Component* component;
        private: 
            bool active = false;
        }; 

        // other stuff may be dependent on the multibus, so start it first
        bool IsEarlyStart() override { return true; };

        // our slots
        Slot slots[MULTIBUS_MAX_SLOTS];
        
        // this simulates the action of the user inserting a slot into the Multibus backplane.
        bool AddSlot(Slot slot, int32_t id);

        uint8_t OnRead8(size_t addr) override;
        uint16_t OnRead16(size_t addr) override;
        uint32_t OnRead32(size_t addr) override;
        void OnWrite8(size_t addr, uint8_t value) override;
        void OnWrite16(size_t addr, uint16_t value) override;
        void OnWrite32(size_t addr, uint32_t value) override;
        // Fire a shared MultiBus IRQ.
        void FireMultibusIRQ(int32_t number);
    private:
        // This is an optimisation, because of the way our bus modelling works we can't actually reliably determine what slot is being written to or read from
        // Since multibus stuff needs to share irq we filter everything through the multibus class. 
        // Let's store the last read and written slot and cache it so we don't need to iterate it. POinter because it needs to be a nullptr.
        Slot* lastSlotRead = nullptr;
        Slot* lastSlotWritten = nullptr;
        
        bool UseCachedReadSlot(size_t addr);
        bool UseCachedWriteSlot(size_t addr);

        bool SetCachedReadSlot(size_t addr);
        bool SetCachedWriteSlot(size_t addr);

        // THE CPU, so we can fire an irq
        ComponentCPU* cpu; 
    
    };
};