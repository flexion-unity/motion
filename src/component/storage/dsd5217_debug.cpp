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
        // TODO: Add a tab for each supported device..
        if (ImGui::Begin("DSD 5217 Disk, Floppy and Tape Controller", &enabled))
        {
            DSD5217* dsd5217 = (DSD5217*)component;

            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "Wake Up Block (WUB)");
            ImGui::Text("Extended Addressing: %s", (dsd5217->wub.extension == DSD5217_24BIT_ADDRESSING) ? "True" : "False");
            ImGui::Text("CCB Pointer: 0x%05x", dsd5217->wub.ccbPtr & 0xFFFFF0); // can't just and this when its set
            
            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "Channel Control Block (CCB)");
            ImGui::Text("Channel 1 Busy: %s", (dsd5217->ccb.busy) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("Channel 2 Busy: %s", (dsd5217->ccb.busy2) ? "True" : "False");
            ImGui::Text("Channel 1 Control Word: 0x%04x", dsd5217->ccb.ccw1);
            ImGui::SameLine();
            ImGui::Text("Channel 2 Control Word: 0x%04x", dsd5217->ccb.ccw2);
            ImGui::Text("CIB Pointer: 0x%05x", dsd5217->ccb.cibPtr & 0xFFFFF0);
            ImGui::Text("CP Pointer: 0x%05x", dsd5217->ccb.cpPtr & 0xFFFFF0);
            ImGui::Text("Control Pointer: 0x%05x", dsd5217->ccb.controlPtr & 0xFFFFF0);
            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "Controller Invocation Block (CIB)");
            ImGui::Text("Command Semaphore: 0x%05x", dsd5217->cib.commandSemaphore);
            ImGui::Text("Status Semaphore: 0x%05x", dsd5217->cib.statusSemaphore);
            ImGui::SameLine();
            ImGui::Text("Operation Status: 0x%05x", dsd5217->cib.opStatus);
            ImGui::Text("IOPB Pointer: 0x%05x", dsd5217->cib.iopbPtr & 0xFFFFF0);
            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "I/O Parameter Block (IOPB)");

            switch (dsd5217->iopb.deviceCode)
            {
                case DSD5217_DEVICE_CODE_FLOPPY:
                    ImGui::Text("Device Code: Floppy Drive");
                    break;
                case DSD5217_DEVICE_CODE_HDD:
                    ImGui::Text("Device Code: Hard Drive (ST-506)");
                    break;
                case DSD5217_DEVICE_CODE_QIC:
                    ImGui::Text("Device Code: Tape (QIC)");
                    break;
                case DSD5217_DEVICE_CODE_TAPE:
                    ImGui::Text("Device Code: Tape (Streaming)");
                    break;
            
            }

            ImGui::Text("Actual Transfers So Far: 0x%04x", dsd5217->iopb.actualTransfers);
            ImGui::Text("Unit: 0x%04x", dsd5217->iopb.unit);
            ImGui::SameLine();
            ImGui::Text("Cylinder: 0x%04x", dsd5217->iopb.cylinder);
            ImGui::SameLine();
            ImGui::Text("Head: 0x%04x", dsd5217->iopb.head);
            ImGui::SameLine();
            ImGui::Text("Sector: 0x%04x", dsd5217->iopb.sector);
            // TODO: visual 
            ImGui::Text("Function: 0x%04x", dsd5217->iopb.function); 
            ImGui::Text("Modifier: 0x%02x", dsd5217->iopb.modifier); 
            ImGui::Text("RBC: 0x%04x", dsd5217->iopb.rbc);  

            ImGui::Text("General Pointer: 0x%05x", dsd5217->iopb.generalPtr & 0xFFFFF0); 
            ImGui::Text("Data Buffer Pointer: 0x%05x", dsd5217->iopb.dba & 0xFFFFF0);

            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "INIB / Data Buffer Information");

            ImGui::Text("Bytes per Sector: 0x%04x", (dsd5217->inist.inib.bytesPerSectorHigh << 8) 
            | (dsd5217->inist.inib.bytesPerSectorLow));

            if (dsd5217->iopb.deviceCode == DSD5217_DEVICE_CODE_FLOPPY)
                ImGui::Text("Removable Heads: %d", dsd5217->inist.inib.removableHeads); 
            else
                ImGui::Text("Fixed Heads: %d", dsd5217->inist.inib.fixedHeads); 
            ImGui::Text("# Cylinders: %d", dsd5217->inist.inib.nrCylinders);
            ImGui::SameLine(); 
            ImGui::Text("Alt Cylinders: %d", dsd5217->inist.inib.numberOfAlternateCylinders); 
            ImGui::Text("Sectors per Track: %d", dsd5217->inist.inib.sectorsPerTrack); 
            ImGui::Text("Fixed Heads: %d", dsd5217->inist.inib.fixedHeads); 
            ImGui::SameLine();
        }

        ImGui::End();
    };
}