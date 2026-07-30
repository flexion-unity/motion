
#include <coherent/coherent_gui_imgui.hpp>
#include <component/ip2/ip2_mmu.hpp>

namespace Motion
{
    void CoherentExtensionIP2MMU::DrawPagetableUI()
    {
        IP2MMU* mmu = (IP2MMU*)component;

        if (ImGui::Begin("Pagetable Viewer", &pagetableUiEnabled))
        {
            for (int32_t i = 0; i < PAGETABLE_MAX_PAGES; i++)
            {
                uint32_t physicalPageNumber = (mmu->pagetable[i] & 0x1FFF);
                uint32_t protectionInfo = ((mmu->pagetable[i] & 0x30000000) >> 28);

                const char* access = "No Access";
                ImVec4 accessColour = ImVec4(1.0, 0.0, 0.0, 1.0);

                if (protectionInfo == 0x01)
                {
                    access = "Read Access";
                    accessColour = ImVec4(1.0, 0.85, 0.0, 1.0);
                }
                else if (protectionInfo == 0x02)
                {
                    access = "System Access";
                    accessColour = ImVec4(1.0, 0.85, 0.0, 1.0);
                }
                else if (protectionInfo == 0x03)
                {
                    access = "Read/Write Access";
                    accessColour = ImVec4(0.0, 1.0, 0.0, 1.0);
                }

                bool referenced = (mmu->pagetable[i] & 0x40000000);
                bool changed = (mmu->pagetable[i] & 0x80000000);

                ImGui::Text("Page %d: Physical Pagenum %d -", i, physicalPageNumber);
                ImGui::SameLine();
                ImGui::TextColored(accessColour, "Access Mode: %s -", access);
                ImGui::SameLine();
                ImGui::Text("Referenced = %d; Changed = %d", referenced, changed);
            }
        }
        
        ImGui::End();
    }

    void CoherentExtensionIP2MMU::AddUI()
    {
        IP2MMU* mmu = (IP2MMU*)component;

        ImGui::SetNextWindowSize(ImVec2(400, 200));

        if (ImGui::Begin("MMU State", &enabled))
        {
            ImGui::Text("OS Base [0x%x]:          0x%x", REG_OS_BASE, mmu->osBase);
            ImGui::Text("Status [0x%x]:           0x%x", REG_STATUS, mmu->status);
            ImGui::Text("Parity [0x%x]:           0x%x", REG_PARITY, mmu->parity);
            ImGui::Text("Multibus Protect [0x%x]: 0x%x", REG_MULTIBUS_PROTECT, mmu->multibusProtect);
            ImGui::Text("Text/Data Base [0x%x]:   0x%x", REG_TEXTDATA_BASE, mmu->textdataBase);
            ImGui::Text("Text/Data Limit [0x%x]   0x%x", REG_TEXTDATA_LIMIT, mmu->textdataLimit);
            ImGui::Text("Stack Base [0x%x]:       0x%x", REG_STACK_BASE, mmu->stackBase);
            ImGui::Text("Stack Limit [0x%x]:      0x%x", REG_STACK_LIMIT, mmu->stackLimit);

            if (ImGui::Button("View Pagetable"))
                pagetableUiEnabled = true; 

            if (pagetableUiEnabled)
                DrawPagetableUI();
        }

        ImGui::End();
    }
}