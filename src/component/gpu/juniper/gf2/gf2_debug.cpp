#include <component/gpu/juniper/gf2/gf2.hpp>

namespace Motion
{
    void CoherentExtensionGF2::AddUI()
    {
        ImGui::SetNextWindowSize(ImVec2(520, 700), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("GF2 System", &enabled))
        {
            if (ImGui::BeginTabBar("GF2MainTabBar"))
            {
                if (ImGui::BeginTabItem("GE"))
                {
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("FBC"))
                {
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("BPC"))
                {
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Command History"))
                {
                    ImGui::EndTabItem();
                }               
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }
}