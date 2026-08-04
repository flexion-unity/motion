#include <component/gpu/juniper/dc4/dc4.hpp>

namespace Motion
{
    void CoherentExtensionDC4::AddUI()
    {
        if (ImGui::Begin("DC4 State", &enabled))
        {
            DC4* dc4 = (DC4*)component;

            bool singleMapMode = (dc4->flags & 0x20);

            // iamadc4
            if (singleMapMode)
                ImGui::TextColored(CoherentUI::COLOUR_HEADER, "DC4: Single-map Mode. Flags:");
            else
                ImGui::TextColored(CoherentUI::COLOUR_HEADER, "DC4: Multi-map Mode, mapped colourmap %d. Flags:", (dc4->flags & 0x0F));

            ImGui::Text("Mapped colourmap %d", (dc4->flags & 0x0F));
            ImGui::Text("Bus addrmap: %d", (dc4->flags & DC4_FLAG_BUS_ADDRMAP));
            ImGui::Text("Single or multimode (reg addrmap): %s", (dc4->flags & DC4_FLAG_REG_ADDRMAP) ? "Multimap" : "Singlemap");
            ImGui::Text("RGB mode: %s", (dc4->flags & DC4_FLAG_RGB_MODE) ? "true" : "false");
            ImGui::Text("Use upper half: %s", (dc4->flags & DC4_FLAG_USE_UPPER_HALF) ? "true" : "false");
            ImGui::Text("OPTCLK enabled: %s", (dc4->flags & DC4_FLAG_OPTIONAL_CLOCK) ? "true" : "false");
            ImGui::Text("Pipeline Depth: %d", (dc4->flags & DC4_FLAG_PIPELINE_DEPTH_4) ? 4 : 2);
            ImGui::Text("Alternate video format: %s", (dc4->flags & DC4_FLAG_PROM) ? "true" : "false");
            ImGui::Text("Freerun mode: %s", (dc4->flags & DC4_FLAG_FREERUN) ? "true" : "false");

            int32_t x = 0, y = 0;

            ImDrawList* windowDrawList = ImGui::GetWindowDrawList();

            // this is a lambda that draws rectangles for us
            auto DrawColour = [](DC4* dc4, ImDrawList* windowDrawList, int32_t i, int32_t& x, int32_t& y)
            {
                uint32_t colour = (dc4->colourMap[i] << 24
                | (dc4->colourMap[i + 0x200] << 16
                | (dc4->colourMap[i + 0x400] << 8
                | 0xFF)));

                windowDrawList->AddRect(ImVec2(x, y), ImVec2(x + 16, y + 16), colour, 0.0f);

                if (!(i & 0x10))
                    y += 16;
                
                x += 16;
            };

            // map 4096 colours
            if (singleMapMode)
            {
                for (int32_t i = 0; i < 4096; i++)
                    DrawColour(dc4, windowDrawList, i, x, y);
            }
            else
            {
                //  probably linearly addressed hopefulyl
                for (int32_t i = 0; i < 16; i++)
                {
                    ImGui::TextColored(CoherentUI::COLOUR_HEADER, "Map %d:", i);

                    // draw each colour
                    for (int32_t j = 0; j < 256; i++)
                        DrawColour(dc4, windowDrawList, (i * j), x, y);
                }
            }
        }

        ImGui::End();
    }
}