
/*
    C    O    H    E    R    E    N    T
    Extensible Emulator Debugging Tools!

    Coherent is an extensible debugger for emulators that is intended to allow the debugging of multiple types of CPU cores in an easy way.

    coherent_gui_imgui.hpp: CoherentUI (Dear imgui implementation)
*/

#pragma once
#include <Motion.hpp>
#include <imgui.h>

namespace Motion 
{    
    #define LOGBUF_MAX_SIZE         16384
    #define LOGBUF_PURGE_SIZE       2048

    class CoherentUI
    {
    public:
        /// @brief The current UI style.
        enum UIStyle
        {
            Default = 0,
            MEX = 1,
            Experimental = 2,
        };

        static void InitStyle(UIStyle style);
        static void InitStyleDefault(UIStyle style, ImGuiStyle& styleObj);
        static void InitStyleMex(UIStyle style, ImGuiStyle& styleObj);
        static void InitStyleExperimental(UIStyle style, ImGuiStyle& styleObj);

        static void LeftAlign();
        static void CentreAlign(ImVec2 contentSize);
        static void RightAlign(ImVec2 contentSize);

        enum GuardWindowType
        {
            GuardWindowWatchpoint = 0,
            GuardWindowBreakpoint = 2,
        };

        static void DrawGuardWindow(GuardWindowType windowType, ImVec2 size);
        static void DrawStackWindow(ImVec2 size);

        // Methods for drawing specific user interfaces

        static void DrawMainWindow();
        static void DrawLogWindow();
        static void DrawAboutWindow();

        // SHOULD NOT BE PUBLIC, but because of some things with the design of SSLS 5, it is.
        static void AddTextToLogWindowBuffer(const char* str);

        static inline bool aboutActive = false;

        // todo: should change based on theme. but the theme system should be redesigned
        static constexpr ImVec4 COLOUR_HEADER = ImVec4(0.1f, 0.8f, 1.0f, 1.0f);

    private: 
        inline static char addrBufForWatchpoints[STRING_MAX_LONG] = {0};
        inline static char addrBufForCatchpoints[STRING_MAX_LONG] = {0};
        inline static char addrBufForBreakpoints[STRING_MAX_LONG] = {0};

        /// @brief internal thing used to store the emulator log. need a cache
        inline static char logBuffer[LOGBUF_MAX_SIZE] = {0};

        static inline bool alreadySetStyle = false;
    };
}