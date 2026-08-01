/*
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    serial.cpp: Serial I/O (see serial.hpp)
*/

#include <component/serial/serial.hpp>

namespace Motion
{
    void SerialLine::SendByte(uint8_t data)
    {
        // send something to use the tx log

        txLog += (char)data;

        if (txLog.size() > SERIAL_TXLOG_MAX_SIZE)
            txLog.erase(0, SERIAL_TXLOG_PURGE_SIZE);

        FireTransmitEvent(data);
    }

    void SerialLine::ClearTxLog()
    {
        txLog.clear();
    }

    bool SerialLine::TryReceiveByte(uint8_t& data)
    {
        if (rxQueue.empty())
            return false;

        data = rxQueue.front();
        rxQueue.pop();

        FireReceiveEvent(data);
        return true;
    }

    void SerialLine::AddRxByte(uint8_t data)
    {
        rxQueue.push(data);
    }

    void SerialLine::AddRxString(const char* str)
    {
        if (!str)
            return;

        for (const char* c = str; *c; c++)
            AddRxByte((uint8_t)*c);
    }

    void SerialLine::FireReceiveEvent(uint8_t data)
    {
        SerialReceiveEvent event = SerialReceiveEvent();
        event.lineId = id;
        event.data = data; 

        EventSystem::FireEvent(event);
    }

    void SerialLine::FireTransmitEvent(uint8_t data)
    {
        SerialTransmitEvent event = SerialTransmitEvent();
        event.lineId = id;
        event.data = data; 

        EventSystem::FireEvent(event);
    }
}
