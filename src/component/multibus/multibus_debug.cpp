/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    multibus_debug.cpp: Coherent extension for Multibus
*/

#include <component/multibus/multibus.hpp>

namespace Motion
{
    void CoherentExtensionMultibus::AddUI()
    {
        Multibus* multibus = (Multibus*)component;

        int32_t slotId = 0;
        
        if (ImGui::Begin("Multibus State"))
        {
            for (Multibus::Slot& slot : multibus->slots)
            {
                if (!slot.active)
                    ImGui::TextColored(ImVec4(0.6, 0.6, 0.6, 1.0), "Slot %d: not active", slotId);
                else
                    ImGui::TextColored(ImVec4(1.0, 1.0, 1.0, 1.0),
                    "Slot %d: I/O 0x%lX-0x%lX: %s", slotId, slot.ioStart, slot.ioEnd, slot.component->GetName());

                slotId++;
            }
        }

        ImGui::End();
    };
};
