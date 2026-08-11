#include <base/cmdline/cmdline.hpp>

namespace Motion
{
    void CommandLine::Parse(int32_t argc, char** argv)
    {
        CommandLine::argc = argc;
    
        for (int32_t i = 0; i < argc; i++)
        {
            std::array<char, STRING_MAX_COMMAND_LINE> cmd{};
            strncpy(cmd.data(), argv[i], STRING_MAX_COMMAND_LINE);
            commands.push_back(cmd);

            // parse early cvars
            if ((argc - i) > 2)
            {
                if (!strcmp(argv[i], "+set"))
                    Cvar::Add(argv[i + 1], argv[i + 2]);
            }

        }
    }

    bool CommandLine::Present(const char* commandString)
    {
        for (auto& command : commands)
        {
            if (!strcmp(command.data(), commandString))
                return true; 
        }

        return false; 
    }

    constexpr char* CommandLine::Argv(int32_t c)
    {
        if (c < 0
        || c >= argc)
            Logger::Log("Attempted to obtain invalid command-line argument ID!", LogChannels::Warning);

        return commands[c].data();
    }

    void CommandLine::PrintHelp()
    {
        std::cout <<
            "m  o  t  i  o  n\n"
            "The SGI Emulator\n"
            "\n"
            "Copyright (c) 2026 starfrost\n"
            "\n"
            "Usage:\n"
            "  motion [options]\n"
            "\n"
            "Options:\n"
            "  -h, --help                     Show this help message and exit\n"
            "  +set <cvar> <value>            Set a console variable (cvar) before startup.\n"
            "                                 Can be repeated to set multiple cvars.\n"
            "\n"
            "Common cvars (set with +set <name> <value>):\n"
            "  vidScale <n>                    UI/window scale factor, e.g. 1, 1.5, 2 (default: 1).\n"
            "  logChannels <mask>              Log channel bitmask override (default: -1, i.e. unchanged)\n"
            "  logDestinations <mask>          Log destination bitmask override (default: -1, i.e. unchanged)\n"
            "  skipLauncher <0|1>              Skip the launcher and go straight to emulation (default: 0)\n"
            "  startPaused <0|1>               Start the debugger paused (default: 1)\n"
            "  machineName <name>              Machine to emulate (default: iris3130)\n"
            "  ramInstalled <bytes>            Amount of system RAM to install, in bytes (default: 16777216)\n"
            "  profileFolder <path>            Path to the user profile/config folder (default: ./profile)\n"
            "\n"
            "Example:\n"
            "  motion +set vidScale 2 +set skipLauncher 1\n";
    }
}