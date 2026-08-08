/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    machine.hpp: Shared code between all machines.
*/

#pragma once

#include <Motion.hpp>
#include <component/addrspace.hpp>
#include <component/component.hpp>
#include <render/render.hpp>

namespace Motion
{
    /// the amount of ram in the system
    extern Cvar* ramInstalled;
    
    class Machine
    {
        #define COMPONENTS_INITIAL_RESERVED     16  
        #define MACHINE_CONFIG_PATH             "Machine.cfg"
        #define LOG_PREFIX_EMU_MACHINE          "Emulation - Machine"

        std::vector<Component*> components;

    public:
        Machine()
        {
            components.reserve(COMPONENTS_INITIAL_RESERVED);
            AddrSpace::maxAddr = 0xFFFFFFFF;                    // 32-bit address space by default
        }

        char* GetFriendlyName() { return friendlyName; };
        void SetFriendlyName(char* friendlyName) { strncpy(friendlyName, this->friendlyName, STRING_MAX_SHORT ); };

        template <std::derived_from<Component> T>   
        T* AddComponent()  
        {
            T* newComponent = new T();

            // test. probably slow.
            Logger::Log(LOG_PREFIX_EMU_MACHINE, std::format("Added component {}", newComponent->GetName()).c_str(), LogChannels::Debug);
            components.push_back(newComponent);

            return newComponent;
        } 

        template <std::derived_from<Component> T>
        T* FindComponentByType()
        {
            for (Component* component : components)
            {
                T* derived = dynamic_cast<T*>(component);
                if (derived)
                    return derived;
            }

            return nullptr;
        }

        /// @brief Start the machine.
        void Start();

        /// @brief Tick each component of the emulation.
        void Tick();

        /// @brief Single step the machine.
        void SingleStep()
        {
            // run tick once
            Tick();
        }

        // should this really be from emulation::event or should we use Emulation::GetMachine::OnEvent...
        void OnEvent(Event& evt);

        /// @brief Called on shutdown.
        void Shutdown();

        /// @brief render something to the scren
        /// @param screen the screen to render to
        void Render(RenderTexture* screen);

        // Virtuals
        virtual void AddComponents() { };
        virtual int32_t GetInternalScreenSizeX() { return 0; };
        virtual int32_t GetInternalScreenSizeY() { return 0; };
        virtual const char* GetName() { return "*****EMULATOR BUG***** Name this Machine"; };

        // not sure if this is a good idea?
        size_t totalRamInstalled;

        // Setters for private fields
    private: 
        char friendlyName[STRING_MAX_LONG] = {0};   // User's name for this machine

        AddrSpace addressSpace;                     // The address space of the machine. Components can use this to read/write memory.  
    };

};