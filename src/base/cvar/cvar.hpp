/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    cvar.hpp: Console variables that allow global configuration 
*/

#pragma once
#include <Motion.hpp>
 
namespace Motion
{
    class Cvar
    {

    public: 
        // Statics (public API)
        static Cvar* Get(const char* name, const char* value);
        static Cvar* Set(const char* name, const char* value);

        // Getters for private fields

        const char* GetName() { return name; };
        float GetValue() { return value; };
        const char* GetString() { return string; }

        // Settes for private fields 

    private: 
        const char* name;
        float value; 
        const char* string;  // the linked list prevents whatever string we put here from being delete.d

        static inline std::unordered_map<std::string, Cvar*, std::hash<std::string_view>, std::equal_to<>> cvars;
        static Cvar* Add(const char* name, const char* value);

        /// @brief Internal method to set a convar
        /// @param name The name of the convar to set
        /// @param value The value
        void SetInternal(const char* newValue);
    };
}