#include <component/gpu/juniper/dc4/dc4.hpp>

namespace Motion
{
    void CoherentExtensionDC4::AddUI()
    {
        if (ImGui::Begin("DC4 State", &enabled))
        {

        }

        ImGui::End();
    }
}