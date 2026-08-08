#include <base/emulation.hpp>
#include <component/component.hpp>
#include <component/memory.hpp>
#include <coherent/coherent.hpp>
#include <base/machine/machines.hpp>

namespace Motion
{    
    Cvar* machineName;

    void Emulation::Init()
    {
        // TEMP
        renderer = new RendererSDL3();
        renderer->Init();
        Coherent::Init();
        Start();
    }   

    /// @brief Determines the users machine type.
    void Emulation::DetermineMachineType()
    {
        machineName = Cvar::Get("machineName", "iris3130");

        bool machineFound = false;

        if (!strcmp(machineName->GetString(), "iris3130"))
        {
            machineFound = true;
            machine = new IRIS3130();
        }

        if (!machineFound)
        {
            Logger::Log(std::format("Invalid machine {} selected. Defaulting to IRIS 3130...", machineName->GetString()).c_str(), LogChannels::Warning);
            machine = new IRIS3130();
        }

        Logger::Log(std::format("Initialising machine {}...", machine->GetName()).c_str());
    }

    void Emulation::Start()
    {
        Logger::Log("Starting emulation...");
    
        DetermineMachineType();
        machine->Start();

        // enter the coherent debugger
        Coherent::Enter();

        Logger::Log("Starting emulation: Starting emulation thread...", LogChannels::Debug);
        running = true; 

        // start the thread
        emuThread = new std::thread(Emulation::Tick);
    }
    
    void Emulation::Frame()
    {
        renderer->FramePreRender();

        if (Coherent::active)
            Coherent::Frame();
            
        renderer->FramePostRender();
    }

    void Emulation::OnEvent(Event& evt)
    {
        machine->OnEvent(evt);
    }
     
    void Emulation::Reset()
    {
        Logger::Log("Resetting emulation...");

        Stop();
        Start();
    }

    void Emulation::SingleStep()
    {
        if (paused)
            machine->SingleStep();
    }

    void Emulation::Tick()
    {
        while (running)
        {
            // do this after processing everything for the current tick
            if (Coherent::active)
                Coherent::Tick();

            if (!paused)
                machine->Tick();
        }
    }

    void Emulation::Stop()
    {
        // Used by both Reset() (which immediately Start()s again) and Shutdown() (which doesn't), so the actual
        // "why" gets logged by whichever of those called us rather than here.

        // make sure the machine is joinable
        SetRunning(false);
        SetPaused(false);

        if (emuThread->joinable())
            emuThread->join();

        Coherent::Leave();
        machine->Shutdown();
        AddrSpace::Shutdown();
    }

    void Emulation::Shutdown()
    {
        Coherent::Shutdown();
        Stop();
        renderer->Shutdown();

        delete machine; 
    }
}