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
        ImGui::SetNextWindowSize(ImVec2(550, 400));

        // so we can print a message if something is missing
        bool ioFound = false, memFound = false;
        // utility: keep track of mapping #
        int32_t mappingNrForSlot = 0, lastSlotMapped = -1; // no slot -1

        if (ImGui::Begin("Multibus State", &enabled))
        {
            for (Multibus::SlotMapping& slot : multibus->slotMappings)
            {
                if (slot.id != lastSlotMapped)
                {
                    lastSlotMapped = slot.id;
                    mappingNrForSlot = 0;
                }

                ImGui::Text("Slot %d mapping %d:", slot.id, mappingNrForSlot);

                if (slot.memStart && slot.memEnd)
                {
                    ImGui::Text("Memory (within 1mb region): %05lx-%05lx", slot.memStart, slot.memEnd);
                    memFound = true; 
                }    

                if (slot.ioStart && slot.ioEnd)
                {
                    ImGui::Text("I/O: %08lx-%08lx", slot.ioStart, slot.ioEnd);
                    ioFound = true;
                }

                if (!memFound && !ioFound)
                {
                    ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), "***** No allocated mapping *****");
                }

                ImGui::Text("Component: %s", slot.component->GetName());
                ImGui::NewLine();

                mappingNrForSlot++;
            }
        }
        
        ImGui::End();
    };
};
