/*
Motion - The SGI Emulator!
Copyright (C) 2026 starfrost
*/

#include <Motion.hpp>
#include <base/emulation.hpp>
#include <base/profile/profile.hpp>
#include <iostream>

namespace Motion
{
    /// @brief Fatal error function
    /// @return Emulator shuts down after this runs
    void fatal()
    {
        Emulation::SetRunning(false);
    }

    void mainThread()
    {
        while (Emulation::IsRunning())
            Emulation::Frame();

        Logger::Log("Shutting down...");

        // shut down the emulation
        Emulation::Shutdown();
        Logger::Shutdown();
    }
    
    Cvar* logChannels;
    Cvar* logDestinations;

    int main(int argc, char** argv)
    {
        Logger::settings.SetAppName(APP_NAME);
        Logger::settings.SetDestinations((LogDestination)(LogDestination::Stdout | LogDestination::File));
        Logger::settings.SetFatalFunction(fatal);
        Logger::settings.SetChannelMask((LogChannels)(LogChannels::Debug | LogChannels::Message | LogChannels::Warning | LogChannels::Error | LogChannels::FatalError | LogChannels::UnsafeShutdown));
        Logger::settings.sendAnsiCodesToFile = false;
        Logger::settings.postLogMessageIgnoresAnsiCodes = true; //coherent
        Logger::Init();
        CommandLine::Parse(argc, argv);                     // parse command line

        logChannels = Cvar::Get("logChannels", "-1");
        logDestinations = Cvar::Get("logDestinations", "-1");

        if (logChannels->GetValue() != -1)
            Logger::settings.SetChannelMask((LogChannels)(int32_t)logChannels);

        if (logDestinations->GetValue() != -1)
            Logger::settings.SetDestinations((LogDestination)(int32_t)logChannels);

        Logger::Log(APP_NAME " v" APP_VERSION " " APP_BUILD_DATE);
        Logger::Log(APP_SIGNON, LogChannels::Message);
        Profile::Init();                                    // init config for user profile
        Emulation::Init();                                  // start emulation thread

        mainThread();

        return EXIT_SUCCESS;
    }
}

int main(int argc, char** argv)
{
    return Motion::main(argc, argv);
}