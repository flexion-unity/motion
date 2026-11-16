#include <component/cpu/mips/mips1.hpp>

namespace Motion
{
    void MIPS::Tick()
    {
        uint32_t opcode = AddrSpace::ReadU32(pc);
        
        pc++;
    }
    

    void MIPS::DecodeI(uint32_t opcode) // DECODE I-Type instruction
    {

    }          

    void MIPS::DecodeJ(uint32_t opcode) // DECODE J-Type instruction
    {

    }        

    void MIPS::DecodeR(uint32_t opcode) // DECODE R-Type instruction
    {

    }       
}; 