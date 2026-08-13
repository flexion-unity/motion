#include <base/program.hpp>
#include <launcher/launcher.hpp>
#include <base/profile/profile.hpp>

namespace Motion
{
    void Launcher::Start()
    {  

    }

    void Launcher::RenderGridItem(ImVec2 size, Cvar* cvar, GridCvarType type, const char* friendlyName)
    {
        if (ImGui::BeginChild("##gridItem", size))
        {
            const char* name = cvar->GetName();
            ImGui::Text("%s: ", friendlyName); 
            ImGui::SameLine();

            bool checkboxHit = false; 
            int32_t intValue = 0;
            float floatValue = 0;
            char buf[STRING_MAX_CVAR] = {0};

            char nameBuf[STRING_MAX_SHORT];

            switch (type)
            {
                case GridCvarType::Boolean:
                    snprintf(nameBuf, STRING_MAX_SHORT, "##Checkbox%d", lastImguiNum);
                    if (ImGui::Checkbox(nameBuf, &checkboxHit))
                    {
                        // because our crappy cvar api depends on strings we have to do this
                        (checkboxHit) ? Cvar::Set(name, "1") : Cvar::Set(name, "0");
                    }
                    break;
                case GridCvarType::Integer:
                    snprintf(nameBuf, STRING_MAX_SHORT, "##IntEntry%d", lastImguiNum);
                    if (ImGui::InputInt(nameBuf, &intValue))
                    {
                        
                    }
                    break;
                case GridCvarType::Float:
                    snprintf(nameBuf, STRING_MAX_SHORT, "##FloatEntry%d", lastImguiNum);
                    if (ImGui::InputFloat(nameBuf, &floatValue))
                    {
                        
                    }
                    break;
                case GridCvarType::String:
                    snprintf(nameBuf, STRING_MAX_SHORT, "##StringEntry%d", lastImguiNum);
                    if (ImGui::InputTextWithHint(nameBuf, "String value...", buf, STRING_MAX_LONG, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        Cvar::Set(name, buf);
                    }
                    break;
            }

            // we don't care about the name or label but imgui does.
            // so increment this so imgui doesn't complain about duplicated elementssss

            lastImguiNum++;

        } 

        ImGui::EndChild();
    }
     
    void Launcher::Frame()
    {
        Program::GetRenderer()->FramePreRender();

        if (ImGui::Begin("Launcher"))
        {
            // reset the number
            lastImguiNum = 0; 

            ImGui::SetWindowFontScale(2.0f);
            ImGui::Text("Motion Launcher");
            ImGui::SetWindowFontScale(1.0f);

            ImVec2 gridEntrySize = ImVec2(400, 250);

            RenderGridItem(gridEntrySize, machineName, GridCvarType::String, "Machine Name");
            RenderGridItem(gridEntrySize, profileFolder, GridCvarType::String, "Profile Folder");
            ImGui::SameLine();
            RenderGridItem(gridEntrySize, ramInstalled, GridCvarType::Integer, "RAM (bytes)");
            ImGui::SameLine();
            RenderGridItem(gridEntrySize, numBitplanes, GridCvarType::Integer, "Bitplanes (4 = 1 BP3 board)");
            ImGui::SameLine();
            RenderGridItem(gridEntrySize, startPaused, GridCvarType::Boolean, "Start Paused");
            RenderGridItem(gridEntrySize, skipLauncher, GridCvarType::Boolean, "Skip Launcher (only effective on next boot)");
            ImGui::SameLine();
            RenderGridItem(gridEntrySize, skipLauncher, GridCvarType::Float, "Video Scale (1-4)");
            ImGui::SameLine();
            RenderGridItem(gridEntrySize, skipLauncher, GridCvarType::Integer, "Log Channel Mask");
            ImGui::SameLine();
            RenderGridItem(gridEntrySize, skipLauncher, GridCvarType::Integer, "Log Destination Mask");
            
            ImGui::NewLine();
            if (ImGui::Button("Go"))
                Program::SetState(ProgramState::Emulation);
        }
        
        ImGui::End();

        Program::GetRenderer()->FramePostRender();
    }
};