/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    uc4_debug.cpp: UC4's coherent extension
*/

#include <component/gpu/juniper/uc4/uc4.hpp>

namespace Motion
{
    void CoherentExtensionUC4::AddUI()
    {
        UC4* uc4 = (UC4*)component;

        if (ImGui::Begin("UC4 State", &enabled))
        {
            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "Configuration");
            ImGui::Text("Board Enabled: %s", (uc4->ucr & UC4_UCR_BOARDENAB) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("Multibus Enabled: %s", (uc4->ucr & UC4_UCR_MBENAB) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("DMA Enabled: %s", (uc4->ucr & UC4_UCR_DMAENAB) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("Interrupts Enabled: %s", (uc4->ucr & UC4_UCR_INTRENAB) ? "True" : "False");
            ImGui::Text("VBlank: %s", (uc4->ucr & UC4_UCR_VERTICAL) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("VBlank Intr Enabled: %s", (uc4->ucr & UC4_UCR_VERTINTR) ? "True" : "False");
            ImGui::Text("Busy: %s", (uc4->ucr & UC4_UCR_BUSY) ? "True" : "False");

            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "Buffers");
            ImGui::Text("EDB: 0x%x ECB: 0x%x", uc4->edb, uc4->ecb);
            ImGui::Text("Current X: 0x%x XEB (error?): 0x%x", uc4->xsb, uc4->xeb);
            ImGui::Text("Current Y: 0x%x YEB (error?): 0x%x", uc4->ysb, uc4->yeb);
            ImGui::Text("FMAB: 0x%x", uc4->fmab);
            ImGui::Text("DDASAF: 0x%x DDASAI: 0x%x", uc4->ddasaf, uc4->ddasai);
            ImGui::Text("DDAEAF: 0x%x DDAEAI: 0x%x", uc4->ddaeaf, uc4->ddaeai);
            ImGui::Text("DDASDF: 0x%x DDASDI: 0x%x", uc4->ddasdf, uc4->ddasdi);
            ImGui::Text("DDAEDF: 0x%x DDAEDI: 0x%x", uc4->ddaedf, uc4->ddaedi);
            ImGui::Text("Mode: 0x%x", uc4->mode);
            ImGui::Text("Repeat: 0x%x", uc4->repeat);
            ImGui::Text("Config: 0x%x", uc4->config);

            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "Commands");
        }

        ImGui::End();
    }
}