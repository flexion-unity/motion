#include <base/program.hpp>
#include <launcher/launcher.hpp>

namespace Motion
{
    Cvar* testCvar;

    void Launcher::Start()
    {  

    }

    void Launcher::RenderGridItem(ImVec2 size, Cvar* cvar, GridCvarType type)
    {
        testCvar = Cvar::Get("testCvar", "0");

        if (ImGui::BeginChild("gridItem", size))
        {
            const char* name = cvar->GetName();
            ImGui::Text("%s: ", name); 
            ImGui::SameLine();

            bool checkboxHit = false; 
            int32_t intValue = 0;
            float floatValue = 0;
            char buf[STRING_MAX_LONG] = {0};

            switch (type)
            {
                case GridCvarType::Boolean:
                    if (ImGui::Checkbox("Checkbox", &checkboxHit))
                    {
                        // because our crappy cvar api depends on strings we have to do this
                        (checkboxHit) ? Cvar::Set(name, "1") : Cvar::Set(name, "0");
                    }
                    break;
                case GridCvarType::Integer:
                    if (ImGui::InputInt("IntEntry", &intValue))
                    {
                        
                    }
                    break;
                case GridCvarType::Float:
                    if (ImGui::InputFloat("FloatEntry", &floatValue))
                    {
                        
                    }
                    break;
                case GridCvarType::String:
                    if (ImGui::InputTextWithHint("StringEntry", "String value...", buf, STRING_MAX_LONG, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        
                    }
                    break;
            }
        } 

        ImGui::EndChild();
    }
     
    void Launcher::Frame()
    {
        Program::GetRenderer()->FramePreRender();

        if (ImGui::Begin("Launcher"))
        {
            
            ImGui::SetWindowFontScale(2.0f);
            ImGui::Text("Motion Launcher");
            ImGui::SetWindowFontScale(1.0f);

            ImVec2 gridEntrySize = ImVec2(250, 125);

            RenderGridItem(gridEntrySize, testCvar, GridCvarType::Boolean);

            if (ImGui::Button("Go"))
                Program::SetState(ProgramState::Emulation);
        }
        
        ImGui::End();

        Program::GetRenderer()->FramePostRender();
    }
};