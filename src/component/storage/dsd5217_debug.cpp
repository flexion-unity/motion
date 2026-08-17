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
            DSD5217* dsd5217 = (DSD5217*)component;

            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "Wake Up Block (WUB)");
            ImGui::Text("Extended Addressing: %s", (dsd5217->wub.extension == 7) ? "True" : "False");
            ImGui::Text("CCB Pointer: %05x", dsd5217->wub.ccbPtr);
            
            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "Channel Control Block (CCB)");
            ImGui::Text("Channel 1 Busy: %s", (dsd5217->ccb.busy) ? "True" : "False");
            ImGui::SameLine();
            ImGui::Text("Channel 2 Busy: %s", (dsd5217->ccb.busy2) ? "True" : "False");
            ImGui::Text("Channel 1 Control Word: %04x", dsd5217->ccb.ccw1);
            ImGui::SameLine();
            ImGui::Text("Channel 2 Control Word: %04x", dsd5217->ccb.ccw2);
            ImGui::Text("CIB Pointer: %05x", dsd5217->ccb.cibPtr);
            ImGui::Text("CP Pointer: %05x", dsd5217->ccb.cpPtr);
            ImGui::Text("Control Pointer: %05x", dsd5217->ccb.controlPtr);
            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "Channel Info Block? (CIB)");
            ImGui::Text("Command Semaphore: %05x", dsd5217->cib.commandSemaphore);
            ImGui::Text("Status Semaphore: %05x", dsd5217->cib.statusSemaphore);
            ImGui::SameLine();
            ImGui::Text("Operation Status: %05x", dsd5217->cib.opStatus);
            ImGui::Text("IOPB Pointer: %05x", dsd5217->cib.iopbPtr);
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

            ImGui::Text("Actual Transfers So Far: %04x", dsd5217->iopb.actualTransfers);
            ImGui::Text("Unit: %04x", dsd5217->iopb.unit);
            ImGui::SameLine();
            ImGui::Text("Cylinder: %04x", dsd5217->iopb.cylinder);
            ImGui::SameLine();
            ImGui::Text("Head: %04x", dsd5217->iopb.head);
            ImGui::SameLine();
            ImGui::Text("Sector: %04x", dsd5217->iopb.sector);
            // TODO: visual 
            ImGui::Text("Function: %04x", dsd5217->iopb.function); 
            ImGui::Text("Modifier: %02x", dsd5217->iopb.modifier); 
            ImGui::Text("RBC: %04x", dsd5217->iopb.rbc);  

            ImGui::Text("General Pointer: %05x", dsd5217->iopb.generalPtr); 
            ImGui::Text("INIB Pointer: %05x", dsd5217->iopb.dba);

            ImGui::TextColored(CoherentUI::COLOUR_HEADER, "INIB / Data Buffer Information");

            ImGui::Text("Bytes per Sector: %04x", (dsd5217->inib.bytesPerSectorHigh << 8) 
            | (dsd5217->inib.bytesPerSectorLow));
            ImGui::Text("Fixed Heads: %04x", dsd5217->inib.fixedHeads); 
            ImGui::Text("# Cylinders: %04x", dsd5217->inib.nrCylinders);
            ImGui::SameLine(); 
            ImGui::Text("Alt Cylinders: %04x", dsd5217->inib.numberOfAlternateCylinders); 
            ImGui::Text("Sectors per Track: %04x", dsd5217->inib.sectorsPerTrack); 
            ImGui::Text("Fixed Heads: %04x", dsd5217->inib.fixedHeads); 
            ImGui::SameLine();
            ImGui::Text("Removable Heads: %04x", dsd5217->inib.removableHeads); 
        }

        ImGui::End();
    };
}