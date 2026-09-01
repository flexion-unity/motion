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

    // this makes the register text look like normal text

    void CoherentUI::PushStylelessTextBox()
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0, 0.0, 0.0, 0.0));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, CoherentUI::COLOUR_HEADER);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));    

        ImGui::PushItemWidth(-FLT_MIN);
    }

    void CoherentUI::PopStylelessTextBox()
    {
        ImGui::PopItemWidth();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(2);
    }
}