/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    dsd5217_debug.cpp: Coherent Extension for DSD 5217
*/

#include <component/storage/dsd5217.hpp>

namespace Motion
{
    void CoherentExtensionDSD5217::AddUI()
    {
        if (ImGui::Begin("DSD 5217 Disk Controller", &enabled))
        {

        }

        ImGui::End();
    };
}