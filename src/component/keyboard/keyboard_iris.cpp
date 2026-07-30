/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    keyboard.hpp: It's the keyboard.
    Generic keyboard interface

    On IRIS and early IRIS 4D, the keyboard I/O Interface is DUART0 Channel A. So we will hook to this. But this is the generic keyboard interface.#
    This class mostly exists so Machine::FindComponentByType can find all types of keyboards.
*/

#include <component/keyboard/keyboard_iris.hpp>

namespace Motion
{
    void KeyboardIris::OnEvent(Event& evt)
    {
        if (!duart)
            duart = Emulation::GetMachine().FindComponentByType<DUART68681>();

        switch (evt.type)
        {
            case EventType::SerialTransmit:
                SerialTransmitEvent transmitEvent = *static_cast<SerialTransmitEvent*>(&evt);
            
                if (transmitEvent.lineId != 0)
                    return;

                switch (transmitEvent.data)
                {
                    case KEYBOARD_CONFIG_REQUEST:
                        duart->GetLine(KEYBOARD_DUART_LINE).AddRxByte(KEYBOARD_TYPE_IRIS);
                        break; 
                }
                break;
        }
    }

    void KeyboardIris::Shutdown()
    {
        duart = nullptr;
    }
}