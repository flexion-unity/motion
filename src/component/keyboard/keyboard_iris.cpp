/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    keyboard.hpp: It's the keyboard.
    Generic keyboard interface

    On IRIS and early IRIS 4D, the keyboard I/O Interface is DUART0 Channel A. So we will hook to this. But this is the generic keyboard interface.#
    This class mostly exists so Machine::FindComponentByType can find all types of keyboards.

    Technically this is an Intel 8748 but HLE will be perfectly acceptable for now, we will emulate the MCU (variant of the 8048) later.
*/

#include <component/keyboard/keyboard_iris.hpp>

namespace Motion
{
    void KeyboardIris::OnEvent(Event& evt)
    {
        if (!duart)
        {
            duart = Emulation::GetMachine()->FindComponentByType<DUART68681>();

            // if still no duart return
            if (!duart)
                return;
        }

        if (evt.type == EventType::SerialTransmit)
        {
            SerialTransmitEvent transmitEvent = *static_cast<SerialTransmitEvent*>(&evt);
        
            if (transmitEvent.lineId != 0)
                return;

            // lol
            switch (transmitEvent.data)
            {
                case KEYBOARD_CONFIG_REQUEST:
                    if (!initialised)
                    {
                        // put the kbd type on the bus
                        duart->GetLine(KEYBOARD_DUART_LINE).AddRxByte(KEYBOARD_TYPE_IRIS);
                        initialised = true; 
                    }
                    break;
                default:
                    if (!shutUpLed)
                    {
                        if (transmitEvent.data & 1)
                        {
                            Logger::Log("Just imagine the shiny keyboard LEDs flashing here for now");
                            shutUpLed = true;
                        }
                    }
                    
                    if (!shutUpBeep)
                    {
                        if (!(transmitEvent.data & 1)) // bit 0 beeping
                        {
                            Logger::Log("Just imagine the keyboard beeping here for now");
                            shutUpBeep = true;
                        }
                    }
                    break;
            }
        }
        else if (evt.type == EventType::KeyDown)
        {
            if (initialised)
            {
                KeyDownEvent& keyDownEvent = static_cast<KeyDownEvent&>(evt);
                uint8_t key = sdlToSgi[(SDL_Keycode)keyDownEvent.key];
                
                // bit7 is used for modifiers but only shift and control since this keyboard doesn't even have an al tkey.
                // handle bit
                if (keyDownEvent.key == SDLK_LSHIFT
                && keyDownEvent.mod & SDL_KMOD_LSHIFT)
                {
                    shiftStateL = !shiftStateL;

                    if (shiftStateL)
                        key |= 0x80;
                }

                if (keyDownEvent.key == SDLK_RSHIFT
                && keyDownEvent.mod & SDL_KMOD_LSHIFT)
                {
                    shiftStateR = !shiftStateR;

                    if (shiftStateR)
                        key |= 0x80;
                }

                if (keyDownEvent.key == SDLK_RSHIFT
                && keyDownEvent.mod & SDL_KMOD_LSHIFT)
                {
                    ctrlStateL = !ctrlStateL;

                    if (ctrlStateL)
                        key |= 0x80;
                }

                if (keyDownEvent.key == SDLK_RSHIFT
                && keyDownEvent.mod & SDL_KMOD_LSHIFT)
                {
                    ctrlStateR = !ctrlStateR;

                    if (ctrlStateR)
                        key |= 0x80;
                }

                // send in the key. We need to convert to SGI format
                duart->GetLine(KEYBOARD_DUART_LINE).AddRxByte(key);
            }
        }
    }

    void KeyboardIris::Shutdown()
    {
        duart = nullptr;
    }
}