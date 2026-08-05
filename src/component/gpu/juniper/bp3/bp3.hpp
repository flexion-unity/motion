#pragma once

#include <Motion.hpp>
#include <component/gpu/vram.hpp>

namespace Motion
{
    extern Cvar* numBitplanes;
    
    #define LOG_PREFIX_BP3              "VRAM - BP3"

    class BP3 : public ComponentVRAM
    {
    public:
        void Start() override;
        void Shutdown() override;

        // these get the internal fb size
        
        int32_t GetInternalFbSizeX() { return 1024; };
        int32_t GetInternalFbSizeY() { return 1024; };
        virtual int32_t GetBytesPerPixel() { return 4; }; 

        const char* GetName() override { return "BP3 Bitplaned VRAM"; };

    private:
        // implements our BPs
        uint32_t writeMask;
    };
};