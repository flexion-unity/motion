/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    launcher.hpp: This is the motion launcher. Basically it lets you set the ConVars and save them to a configuration file.

    It's a separate program to the real emulator. 
    It has its own render loop and a bunch of other things.
*/

#pragma once
#include <Motion.hpp>
// separate to coherent
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

namespace Motion
{
    // about window scrolling shizzle
    // sentinel value so we don't constnatly reset every frame
    #define ABOUT_WINDOW_POS_NOT_SET_YET    -10000000.0f 
    // cool ticker text
    #define ABOUT_WINDOW_TICKER_TEXT        "Al Kossow @ bitsavers.org, Dirk W. Hoffmann, CJ, flexion, danifunker, t_r_0_n, & unix-haters.org team" 

    class Launcher
    {
    public: 
        /// @brief starts the launcher
        static void Start();

        /// @brief updates the launcher
        static void Frame(); 

    private:
        /// @brief types of grid convars that we need
        enum GridCvarType
        {
            Boolean = 0,
            Integer = 1,
            Float = 2,
            String = 3,
        }; 

        static void RenderGridItem(ImVec2 size, Cvar* cvar, GridCvarType type, const char* friendlyName);
        inline static int32_t lastImguiNum = 0; 

        // About window stuff
        static void RenderEmulatorAbout();

        struct AboutWindowState
        {
            float scrollPos; // initial STUPID value
            bool enabled;
            float speed; 

            AboutWindowState()
            {
                scrollPos = ABOUT_WINDOW_POS_NOT_SET_YET;
                enabled = false;
                speed = 50.0f;
            }
        }; 

        inline static AboutWindowState aboutWindowState;
    };
}; 
