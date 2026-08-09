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
            ImGui::Text("Bresenham Error Delta (EDB): 0x%x Bresenham Error Correction (ECB): 0x%x", uc4->edb, uc4->ecb);
            ImGui::Text("X start (XSB): %d X end (XEB): %d", uc4->xsb, uc4->xeb);
            ImGui::Text("Y start (YSB): %d Y end (YEB): %d", uc4->ysb, uc4->yeb);
            ImGui::Text("Font Memory Address (FMAB): 0x%x", uc4->fmab);
            ImGui::Text("DDA Start Address F: 0x%x DDA Start Address I: 0x%x", uc4->ddasaf, uc4->ddasai);
            ImGui::Text("DDA End Address F: 0x%x DDA End Address I: 0x%x", uc4->ddaeaf, uc4->ddaeai);
            ImGui::Text("DDA Start Delta High (F): 0x%x DDA Start Delta Low (I): 0x%x", uc4->ddasdf, uc4->ddasdi);
            ImGui::Text("DDA End Delta High (F): 0x%x DDA End Delta Low (I): 0x%x", uc4->ddaedf, uc4->ddaedi);

            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "Mode");
            ImGui::Text("Swizzle: %s", (uc4->mode & UC4_MODE_SWIZZLE) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("Double Buffering: %s", (uc4->mode & UC4_MODE_SWIZZLE) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("Depth Cue: %s", (uc4->mode & UC4_MODE_DEPTH_CUE) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("MB Set Address (?): %s", (uc4->mode & UC4_MODE_MB_SETADDR) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("Line Stipple Repeat (RPB): 0x%x", uc4->repeat);
            ImGui::Text("MB Set Address (?): %s", (uc4->mode & UC4_MODE_MB_SETADDR) ? "True" : "False");

            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "Config");
            ImGui::Text("Display buffer A?: %s buffer B?: %s", 
                (uc4->config & UC4_CFG_DISP_A) ? "True" : "False",
                (uc4->config & UC4_CFG_DISP_B) ? "True" : "False"
            );
            ImGui::Text("Update buffer A?: %s buffer B?: %s", 
                (uc4->config & UC4_CFG_UPDATE_A) ? "True" : "False",
                (uc4->config & UC4_CFG_UPDATE_B) ? "True" : "False"
            );
            ImGui::Text("Screen Mask: %s", (uc4->config & UC4_CFG_SCREEN_MASK)? "True" : "False");
            ImGui::Text("Invert Display: %s", (uc4->config & UC4_CFG_INVERT)? "True" : "False");
            ImGui::Text("Line Just Finished: %s", (uc4->config & UC4_CFG_FINISH_LINE) ? "True" : "False");
            ImGui::Text("FI CD: %s", (uc4->config & UC4_CFG_PFICD) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("FI Read: %s", (uc4->config & UC4_CFG_PFIREAD) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("FI Column: %s", (uc4->config & UC4_CFG_PFICOLUMN) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("FI X Down: %s", (uc4->config & UC4_CFG_PFIXDOWN) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("FI Y Down: %s", (uc4->config & UC4_CFG_PFIYDOWN) ? "True" : "False");
            ImGui::Text("All Pattern: %s", (uc4->config & UC4_CFG_ALLPATTERN) ? "True" : "False");
            ImGui::Text("Pattern is 32x32: %s", (uc4->config & UC4_CFG_ALLPATTERN) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("64x64: %s", (uc4->config & UC4_CFG_ALLPATTERN) ? "True" : "False");

            
            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "Commands");
        }

        ImGui::End();
    }
}