#include <base/program.hpp>
#include <launcher/launcher.hpp>

namespace Motion
{
    void Launcher::Start()
    {  

    }

    void Launcher::Frame()
    {
        Program::GetRenderer()->FramePreRender();

        if (ImGui::Begin("Launcher"))
        {
            if (ImGui::Button("Go"))
                Program::SetState(ProgramState::Emulation);
        }
        
        ImGui::End();
        
        Program::GetRenderer()->FramePostRender();
    }
};