/*
    C    O    H    E    R    E    N    T
    Extensible Emulator Debugging Tools!

    Coherent is an extensible debugger for emulators that is intended to allow the debugging of multiple types of CPU cores in an easy way.

    coherent_editorcpp: Coherent Memory Editor base code.
*/

#include <coherent/coherent_editor.hpp>

namespace Motion
{
    void CoherentEditor::AddUI()
    {
        ImGui::SetNextWindowSize(ImVec2(500, 400));

        if (!settings.buf)
        {
            if (!shutupFatalError) // otherwise it will log every frame
            {
                Logger::Log(COHERENT_LOG_PREFIX, "CoherentEditor will not display because nothing was provided for it to edit (settings.buf == nullptr)", LogChannels::Error);
                shutupFatalError = true;
            }

            return;
        }
        else if (!settings.bufSize)
        {
            if (!shutupFatalError)
            {
                Logger::Log(COHERENT_LOG_PREFIX, "CoherentEditor will not display because the buffer size is 0 bytes (settings.bufSize == 0)", LogChannels::Error);
                shutupFatalError = true;   
            }
        }   

        const char* name = settings.name;

        if (!name)
            name = "Name this editor please";

        if (ImGui::Begin(name, &enabled))
        {
            for (int32_t i = 0; i < settings.loadAtOnce; i += settings.lineSize)
            {
                ImGui::Text("%.8x:\t", i);
                ImGui::SameLine();

                for (int32_t j = 0; j < settings.lineSize; j++)
                {
                    ImGui::Text("%.2x", settings.buf[i + j]);
                    ImGui::SameLine();
                }

                ImGui::NewLine();
            }
        }

        ImGui::End();
    }

    void CoherentEditor::SetDefaultSettings()
    {
        settings.loadAtOnce = 0x10000; //test
        settings.lineSize = 16; 
    }
}; 