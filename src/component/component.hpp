/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    component.hpp: Base class for all components
*/

#pragma once
#include <Motion.hpp>
#include <base/event/event.hpp>
#include <render/render.hpp>

namespace Motion
{
    class Component
    {

    public: 

        /// @brief determines when this component was last ticked.
        uint64_t lastTickNs;

        /// @brief Allows an artificial delay to be implemented for less precisely timed components.
        uint64_t delayNs;

        /// @brief Returns the clock speed of the component in hertz
        /// @return Returns the clock speed of the component in hertz
        virtual uint32_t GetClockSpeed() { return 0; }; 

        virtual void Start() { };
        virtual void Tick() { };                // ticks at clock speed
        virtual void OnEvent(Event& evt) { };   // fire an event
        virtual void Reset() { };               // fired when the emulator starts or resets
        virtual void Shutdown() { };            // fired on shutdown
        
        virtual uint8_t Read8(size_t addr) 
        { 
            Logger::Log(std::format("An 8-bit read from the address 0x{:x} occurred for the component {}, "
                "but this component doesn't implement Component::Read8!", addr, GetName()).c_str(), LogChannels::Warning);

            return 0; 
        };

        virtual uint16_t Read16(size_t addr) 
        { 
            Logger::Log(std::format("A 16-bit read from the address 0x{:x} occurred for the component {}, "
            "but this component doesn't implement Component::Read16!", addr, GetName()).c_str(), LogChannels::Warning);

            return 0; 
        };

        virtual uint32_t Read32(size_t addr) 
        { 
            Logger::Log(std::format("A 32-bit read from the address 0x{:x} occurred for the component {}, "
            "but this component doesn't implement Component::Read32!", addr, GetName()).c_str(), LogChannels::Warning);

            return 0; 
        };

        virtual void Write8(size_t addr, uint8_t value)
        { 
            Logger::Log(std::format("An 8-bit write of 0x{:x} to the address 0x{:x} occurred for the component {}, "
            "but this component doesn't implement Component::Write8!", value, addr, GetName()).c_str(), LogChannels::Warning);
        };

        virtual void Write16(size_t addr, uint16_t value) 
        { 
            Logger::Log(std::format("A 16-bit write of 0x{:x} to the address 0x{:x} occurred for the component {}, "
            "but this component doesn't implement Component::Write16!", value, addr, GetName()).c_str(), LogChannels::Warning);
        };
        
        virtual void Write32(size_t addr, uint32_t value) 
        { 
            Logger::Log(std::format("A 32-bit write of 0x{:x} to the address 0x{:x} occurred for the component {}, "
            "but this component doesn't implement Component::Write32!", value, addr, GetName()).c_str(), LogChannels::Warning);
        };

        /// @brief Called when it is time for this component to render.
        /// @param texture the texture to render to
        virtual void Render(RenderTexture* screen) { }; 

        // Component type determination truth values

        /// @brief get the name of this component. immutable const char*.
        virtual const char* GetName() { return "Name this component by overriding Component::GetName!"; };

        /// @brief an early start component, is dependent on 
        virtual bool IsEarlyStart() { return false; }; 

        virtual ~Component()
        {
            
        }

    };
}