#include <component/gpu/juniper/bp3/bp3.hpp>

namespace Motion
{
    Cvar* numBitplanes;

    void BP3::Start()
    {
        bool invalid = false; 
        const char* invalidMessage = "";
        // Install the number of bitplanes that are required
        numBitplanes = Cvar::Get("numBitplanes", "32");
        realBitplanes = (int32_t)numBitplanes->GetValue();
        
        // BP3 boards each hold 1 bitplane. 
        if (realBitplanes % 4 != 0)
        {
            realBitplanes = ((int32_t)realBitplanes + 3) & ~3;
            invalidMessage = "Number of bitplanes must be a multiple of 4 (one BP3 board is 4 bitplanes)";
            invalid = true; 
        }

        if (realBitplanes > 32)
        {
            invalid = true;  
            invalidMessage = "Only up to 32 bitplanes can be installed.";
            realBitplanes = 32;
        }
        
        if (realBitplanes < 4)
        {
            invalid = true;
            invalidMessage = "At least 4 bitplanes have to be installed.";
            realBitplanes = 4;
        }

        // funny
        if (realBitplanes == 420)
        {
            invalid = true;
            invalidMessage = "Ur vram is a g ThAnG bRO...";
            realBitplanes = 32; 
        }

        if (invalid)
        {
            Logger::Log(LOG_PREFIX_BP3, std::format("Your bitplane setting was rejected because: {}."
                "The system will come up with {} bitplanes.", invalidMessage, realBitplanes).c_str(),
                LogChannels::Warning);
        }

        Logger::Log(LOG_PREFIX_BP3, std::format("There are {} bitplanes.", realBitplanes).c_str());

        // only NOW call vram start once the real number of bitplanes was determined.
        ComponentVRAM::Start();

        // don't need to update to match.
    }

    void BP3::Shutdown()
    {
        ComponentVRAM::Shutdown();
    }
};