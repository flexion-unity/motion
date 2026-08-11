/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    main.cpp: The entry file of Motion. This basically has a very basic state machine that switches
    between sub-programs.
*/

#include <Motion.hpp>
#include <base/emulation.hpp>
#include <base/profile/profile.hpp>
#include <iostream>

namespace Motion
{
    Cvar* logChannels;
    Cvar* logDestinations;
    Cvar* skipLauncher; 

    /// @brief Fatal error function
    /// @return Emulator shuts down after this runs
    void Program::Fatal()
    {
        Emulation::SetRunning(false);
    }

    void Program::MainThread()
    {
        switch (Program::state)
        {
            case ProgramState::Launcher:
                fallthrough; // for now
            case ProgramState::Emulation:
                Emulation::Start();         // start emulation

                while (Emulation::IsRunning())
                    Emulation::Frame();

                Logger::Log("Shutting down...");

                // shut down the emulation
                Emulation::Shutdown();
                Logger::Shutdown();
                break; 
        }
    }

    void Program::SetState(ProgramState state)
    {
        Program::state = state; 
    };

    /// @brief initialise shared program systems
    void Program::Init(int argc, char** argv)
    {
        Logger::settings.SetAppName(APP_NAME);
        Logger::settings.SetDestinations((LogDestination)(LogDestination::Stdout | LogDestination::File));
        Logger::settings.SetFatalFunction(Program::Fatal);
        Logger::settings.SetChannelMask((LogChannels)(LogChannels::Debug | LogChannels::Message | LogChannels::Warning | LogChannels::Error | LogChannels::FatalError | LogChannels::UnsafeShutdown));
        Logger::settings.sendAnsiCodesToFile = false;
        Logger::settings.postLogMessageIgnoresAnsiCodes = true; //coherent
        Logger::Init();
        CommandLine::Parse(argc, argv);                     // parse command line

        logChannels = Cvar::Get("logChannels", "-1");
        logDestinations = Cvar::Get("logDestinations", "-1");
        skipLauncher = Cvar::Get("skipLauncher", "0");

        if (logChannels->GetValue() != -1)
            Logger::settings.SetChannelMask((LogChannels)logChannels->GetValue());

        if (logDestinations->GetValue() != -1)
            Logger::settings.SetDestinations((LogDestination)logChannels->GetValue());

        Logger::Log(APP_NAME " v" APP_VERSION " " APP_BUILD_DATE);
        Logger::Log(APP_SIGNON, LogChannels::Message);
        Profile::Init();                                    // init config for user profile
        Emulation::Init();                                  // start emulation thread
    }

    /// @brief main function
    /// @param argc argc passed from libc main
    /// @param argv argv passed from libc main
    /// @return an exit code. 0 = success, 1 = failure
    int32_t Program::Main(int argc, char** argv)
    {
        Init(argc, argv);

        bool shouldSkipLauncher = (skipLauncher->GetValue() != 0);

        if (shouldSkipLauncher)
            SetState(ProgramState::Emulation);
        else   
            SetState(ProgramState::Launcher);

        MainThread();

        return EXIT_SUCCESS;
    }
}

int main(int argc, char** argv)
{
    return Motion::Program::Main(argc, argv);
}