/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    main.cpp: The entry file of Motion. This basically has a very basic state machine that switches
    between sub-programs.
*/

#include <Motion.hpp>
#include <launcher/launcher.hpp>
#include <base/emulation.hpp>
#include <base/profile/profile.hpp>
#include <base/program.hpp>
#include <render/render.hpp>
#include <render/sdl3/render_sdl3.hpp>
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
        if (state == ProgramState::Emulation)
            Program::running = false; 
    }

    void Program::MainThread()
    {
        switch (Program::state)
        {
            case ProgramState::Launcher:
                Launcher::Frame();
                break;
            case ProgramState::Emulation:
                Emulation::Frame();
                break; 
        }
    }

    void Program::SetState(ProgramState state)
    {
        Program::state = state; 

        // initialise the current state
        switch (Program::state)
        {
            case ProgramState::Emulation:
                Emulation::Init();          // start emulation thread
                Emulation::Start();         // start emulation
                break; 
        }
    };

    /// @brief initialise shared program systems
    void Program::Init(int argc, char** argv)
    {
        CommandLine::Parse(argc, argv); // parse command line early to detect --help

        if (CommandLine::Present("--help") || CommandLine::Present("-h"))
        {
            CommandLine::PrintHelp();
            exit(EXIT_SUCCESS);
        }

        Logger::settings.SetAppName(APP_NAME);
        Logger::settings.SetDestinations((LogDestination)(LogDestination::Stdout | LogDestination::File));
        Logger::settings.SetFatalFunction(Program::Fatal);
        Logger::settings.SetChannelMask((LogChannels)(LogChannels::Debug | LogChannels::Message | LogChannels::Warning | LogChannels::Error | LogChannels::FatalError | LogChannels::UnsafeShutdown));
        Logger::settings.sendAnsiCodesToFile = false;
        Logger::settings.postLogMessageIgnoresAnsiCodes = true; //coherent
        Logger::Init();

        if (logChannels->GetValue() != -1)
            Logger::settings.SetChannelMask((LogChannels)logChannels->GetValue());

        if (logDestinations->GetValue() != -1)
            Logger::settings.SetDestinations((LogDestination)logChannels->GetValue());

        Logger::Log(APP_NAME " v" APP_VERSION " " APP_BUILD_DATE);
        Logger::Log(APP_SIGNON, LogChannels::Message);
        Profile::Init();                                    // init config for user profile

        // TEMP
        renderer = new RendererSDL3();

        // the renderer is dependent on specific information like the real internal fb size of the machine's GPU
        // TODO: a real config system that can get us away from this ?
        renderer->Init();


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

        running = true;

        while (running)
            MainThread();

        Shutdown();

        return EXIT_SUCCESS;
    }

    void Program::Shutdown()
    {
        Logger::Log("Shutting down...");

        if (state == ProgramState::Emulation)
            Emulation::Shutdown();
            
        renderer->Shutdown();
        Logger::Shutdown();
    }

}

int main(int argc, char** argv)
{
    return Motion::Program::Main(argc, argv);
}