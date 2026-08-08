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
        if (ImGui::Begin("UC4 State", &enabled))
        {

        }

        ImGui::End();
    }
}