/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    multibus.hpp: The Intel Multibus bus.
    This is the bus that is used for everything on the early IRISes, and most things on the later IRIS 3000.

    Most likely it was inheited from the SUN design that SGI bought back in '82. PM1/PM2/IP1 boards are fully multibus, IP2 boards,
    except for the CPU, GPU and FPU (?) (thees use their own private bus.)

    TODO:
        Cards that use memory space, not just I/O space.
        Parity Control
        MB Protection
*/

#pragma once
#include <Motion.hpp>
#include <coherent/coherent.hpp>
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

    class CoherentExtensionMultibus : public CoherentExtension
    {
    public: 
        CoherentExtensionMultibus(Component* owner) : CoherentExtension(owner) {}

        void AddUI() override; 
    }; 

    class Multibus : public Component
    {
        // friend so we can access random etc
        friend class CoherentExtensionMultibus;

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
            friend class CoherentExtensionMultibus; 

        public: 
            uint32_t irq; 

            // Yes we are hardcoding the addresses.
            // HOW do these devices find out whta part of the address space they decode ???? WHAT ??? NO BARs ????
            size_t memStart;
            size_t memEnd;
            size_t ioStart;
            size_t ioEnd; 
            Component* component;

            Slot(Component* component)
            {
                this->component = component;
            }

        private: 
            bool active = false;

            // parameterless constructor should be private
            Slot()
            {

            }
        }; 

        // other stuff may be dependent on the multibus, so start it first
        bool IsEarlyStart() override { return true; };

        // our slots
        Slot slots[MULTIBUS_MAX_SLOTS];
        
        // this simulates the action of the user inserting a slot into the Multibus backplane.
        // NOTE: no attempt is made to prevent the addresses overlapping. the first slot in the range will be used.
        bool AddSlot(Slot slot, int32_t id);

        // for OS initiated remaping
        void UpdateSlotIOMapping(int32_t id, size_t addrStart, size_t addrEnd);
        void UpdateSlotMemMapping(int32_t id, size_t addrStart, size_t addrEnd);

        uint8_t Read8(size_t addr) override;
        uint16_t Read16(size_t addr) override;
        uint32_t Read32(size_t addr) override;
        void Write8(size_t addr, uint8_t value) override;
        void Write16(size_t addr, uint16_t value) override;
        void Write32(size_t addr, uint32_t value) override;
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

        // these are never meant to fail. MB writes should not occur unmapped.
        bool SetCachedReadSlot(size_t addr);
        bool SetCachedWriteSlot(size_t addr);

        // THE CPU, so we can fire an irq
        ComponentCPU* cpu; 
        // our coherent extension
        CoherentExtensionMultibus* multibusExtension;
    
    };
};