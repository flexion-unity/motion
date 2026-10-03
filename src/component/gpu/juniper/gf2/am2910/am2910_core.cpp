/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    am2910.cpp: The AMD Am2910 Microcode Sequencer Implementation
    This one uses the am2903 to perform actions 
     
    Source: https://www.datasheets360.com/pdf/-6069213202016663880
*/

#include <component/gpu/juniper/gf2/am2910/am2910.hpp>

namespace Motion
{
    /// @brief Start exeuction of the am2910. Default is at 0x0.
    /// @param pcReg 
    void AM2910::Start(uint16_t pcReg)
    {
        this->pcReg = pcReg;
        running = true; 

        // WHAT IS PCCTR ?? WHY ???
    }

    void AM2910::Tick()
    {
        // next the next state
        uint16_t next = (ucode->data[pcReg][0]);

        pcReg += 2; 

    }
    
    uint16_t AM2910::StackPush()
    {
        return 0xFF; 
    }

    void AM2910::StackPop()
    {

    }

    /// @brief Basically goes HEY 2903, EXECUTE THIS MICROCODE NOW!
    /// @param nextUcode the microcode instruction to execute
    void AM2910::YellAt2903(uint16_t nextUcode)
    {
        the2903->ExecuteUcode(nextUcode);
    }
}