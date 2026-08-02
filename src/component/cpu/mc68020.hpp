#pragma once

#include <Motion.hpp>
#include <component/component.hpp>
#include <component/cpu/cpu.hpp>
#include <base/emulation.hpp>
#include <component/cpu/mc68020_moira_bridge.hpp>

namespace Motion
{
    #define MOIRA_DISASM_BUF_SIZE    512

    /// @brief Debugger extensions for Lisburn
    class MC68020DebuggerSystem : public CoherentSystem
    {

    public:
        char* DisasmInstruction(size_t start) override;
        size_t GetPC() override;

        /// Initialise an MC68020 DEbugger System
        MC68020DebuggerSystem(MC68020MoiraBridge* bridge)
        {
            this->moiraCpu = bridge;
        }

        // for the coherent stack window

        uint32_t GetStack32(uint32_t offset) override
        {
            return AddrSpace::ReadU32(moiraCpu->reg.sp + (offset << 2));
        };

    private: 
        char disasmBuf[MOIRA_DISASM_BUF_SIZE] = {0};
        MC68020MoiraBridge* moiraCpu;
    };

    class MC68020 : public ComponentCPU
    {
    public: 

        MC68020MoiraBridge moiraCpu;
        // MMU & FPU are off-chip. 68881 + 68882.
        
        //
        // METHODS
        // 

        // rc16 uses this clock speed. sdhould be switchable eventually
        uint32_t GetClockSpeed() override { return 16666670; }; 

        void Start() override;
        void Tick() override;
        void Shutdown() override; 

        // other stuff may be dependent on tih scpu
        bool earlyStart = true; 

        /// @brief get the name of this component. immutable const char*.
        const char* GetName() { return "Motorola MC68020 CPU (Lisburn)"; };

        // not sure if this is right
        void SetIRQ(int32_t irq) override { moiraCpu.setIPL(irq) };

    private:
        MC68020DebuggerSystem* system; 
    };
}