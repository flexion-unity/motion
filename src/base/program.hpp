/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    program.hpp: header file for main program code in main.cpp
*/

#pragma once
#include <Motion.hpp>
#include <render/render.hpp>

namespace Motion
{
    // State of emulator
    // NOTE: This class is only defined in one file as a way o
    enum ProgramState
    {
        Launcher = 0,
        Emulation = 1,
    }; 

    class Program
    {
    public:
        static int32_t Main(int argc, char** argv);

        static ProgramState GetState() { return state; };
        static void SetState(ProgramState state);
        static void Fatal();

        /// @brief holds if the program is running
        inline static bool running;
        
        // getters for privates
        static Renderer* GetRenderer() { return renderer; };

        // setters for privates
        
    private:
        static void Init(int argc, char** argv);
        static void MainThread();

        inline static ProgramState state; 

        /// the renderer
        inline static Renderer* renderer; 


        static void Shutdown();
    };
}