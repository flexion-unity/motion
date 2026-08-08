#include <coherent/coherent.hpp>
#include <coherent/coherent_gui_imgui.hpp>

namespace Motion
{
    void CoherentUI::LeftAlign()
    {
        auto targetCursorX = ImGui::GetWindowSize().x - ImGui::GetContentRegionAvail().x;
        ImGui::SetCursorPosX(targetCursorX);
    }

    void CoherentUI::CentreAlign(ImVec2 contentSize)
    {
        auto targetCursorX = ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x / 2) - (contentSize.x / 2);
        ImGui::SetCursorPosX(targetCursorX);
    }

    void CoherentUI::RightAlign(ImVec2 contentSize)
    {
        auto targetCursorX = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - contentSize.x;
        ImGui::SetCursorPosX(targetCursorX);
    }
}