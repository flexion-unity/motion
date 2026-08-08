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

        // We model our VRAM as a 1024*1024*32 bits and mask all writes to the appropriate number of bitplanes.
        virtual int32_t GetBytesPerPixel() { return 4; }; 

        // Register I/O
        uint8_t OnRead8(size_t addr) override;
        uint16_t OnRead16(size_t addr) override;
        uint32_t OnRead32(size_t addr) override;
        void OnWrite8(size_t addr, uint8_t value) override;
        void OnWrite16(size_t addr, uint16_t value) override;
        void OnWrite32(size_t addr, uint32_t value) override; 
        
        const char* GetName() override { return "BP3 Bitplaned VRAM"; };

    private:
        // implements our BPs
        uint32_t realBitplanes;
        uint32_t writeMask;
    };
};