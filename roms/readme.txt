ROMs go here

M  O  T  I  O  N
The SGI Emulator 

Currently this emulator targets the 68020-based IRIS 3000 series machines only.

Copyright (C) 2026 

A basic guide for using the emulator. May be out of date it is always changing 
Last updated: July 30, 2026 (v0.0730)

Emulator Log window - shows you the log of what is happening
Coherent window - debugger
    TOP BAR:
        Pause CPU - Pause the CPU
        Reset - Reset emulation 
        Step - single step (only if paused)
        
    LEFT BAR:
        Shows register view of the 68020 CPU.

    MIDDLE BAR:
        Shows the next 30 instructions of the 68020 CPU; the currently executed instruction is highlighted in blue.

    RIGHT BAR:
        Breakpoints
            Input an address into the text box and press Add to add a breakpoint.
            Click on a breakpoint to select it (due to IMGUI weirdness, currently it doesn't show a colour while selected.)
            If you have any breakpoints selected clicking remove will remove them.
        Watchpoints
            Will show you the 32-bit value of any memory address you put in.
        Stack
            Shows the top 8 dwords in the stack. A bit useless, since variables and parameters and such are oftne pushed onto the stack.
            Eventually I'll profile calls and such to create a real call stack.

    MENU ITEMS:

    Peripherals - lets you access the peripheral debuggers.
        IP2 MMU - Debug SGI's TTL MMU and view the pagetable.
        DUART - Debug the serial DUARTs.

    Style - lets you change style. The styles currently suck
    System Configuration - reconfigure the IRIS's back panel switches.
    Serial Console - Access the PROM console if enabled and view information about the state of the UARTs, ? is help.

Command line:
+set - set a convar. Follow with a value, "1" to enable "0" to disable.
    logIP2MMU 
        Log the IP2 MMU (a custom TTL job by SGI).
    logIP2DUART 
        Enables debug log messages for the IP2 DUART (two Signetics SCN68681 DUARTs).
    logIP2RTC
        Enables debug log messages for  the IP2 RTC, which is currently a stub emulation. 
    profileLocation - the location of the profile
        RESET the profile - delete every folder in this folder.

        Profile files:
            ip2_sram.bin: Private PROM SRAM.
    promPath
        the path of the PROM (basically the BIOS) to load.
        If you are messing around with different versions of the BIOS oyu can set this
    forceUseSerialMonitor
        Disconnects the keyboard from the emulated machine. This forces the PROM to enter serial communication mode over DUART0 Port B.
        Since the graphics system (and multibus) don't work yet this is the only way to do anything with the machine right now.
    startPaused (default is 1)
        Start the emulator paused, 0 will start the CPU immediately.

Notes:
    "Unmapped write" or "Unmapped read" warnings: Ignore them, they are fine.

Not done:
    - Reconfigurable machines
    - Debugger commands
    - Configuration
    - Most things emulation wise