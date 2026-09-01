M  O  T  I  O  N
The SGI Emulator 

Version 0.3.0
Copyright (C) 2026 starfrost

Currently this emulator targets the 68020-based IRIS 3000 series machines only.
Here's a basic guide for using the emulator:

1. ROMS: 

ROMs go in the ROMs folder

2. THE MAIN WINDOW:

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
            DC4 - Debug the GPU's display contrroller (DC4) board   
            DSD 5217 - Debug the DSD 5217 Disk Controller, if you selected it 
        

    Style - lets you change style. The styles currently suck
    Backpanel Switches - reconfigure the IRIS's back panel switches.
    System Console - Access the PROM console if enabled and view information about the state of the UARTs
        In the PROM ? is help.
        You can also, by deactivating the GF2 Board, boot UNIX over serial.

Command line:
--help, --h, -? - display help
+set - set a convar. Follow with a value, "1" to enable "0" to disable.
    Logging: (warnings and errors are always printed)
        logIP2MMU 
            Log the IP2 MMU (a custom TTL job by SGI).
        logIP2DUART 
            Enables debug log messages for the IP2 DUART (two Signetics SCN68681 DUARTs).
        logIP2RTC
            Enables debug log messages for the IP2 RTC, a Motorola MC146818.
        logDC4
            Enables debug log messages for the Display Controller.
        logUC4
            Enables debug log messages for the Update Controller.
        logChannels
            Provide a custom log channel mask. Default is -1, which means "use the emulator's default settings":

            Masks:
                0x00000001 - Allow Message level logs to be displayed.
                0x00000002 - Allow Warning level logs to be displayed.
                0x00000004 - Allow Error level logs to be displayed.
                0x00000008 - Allow FatalError level logs to be displayed.
                0x00000010 - Allow UnsafeShutdown level logs to be displayed.

        logDestinations
            Provide a custom log destination mask. Default is -1, which means "use the emulator's default settings".

            Masks:
                0x00000001 - Log all messages to stdout.
                0x00000002 - Log all messages to stderr.
                0x00000004 - Log all messages to file.
    Configuration: 

        profileFolder - the location of the profile
            RESET the profile - delete every folder in this folder.
                This will also delete your user data.
        dumpsFolder
            Dumps folder goes here 
            
            Profile files:
                ip2_sram.bin: Private PROM SRAM.
        profileDisk0Path - the path of hard drive 0.
            Must be in the "profile" folder.
        profileDisk1Path - the path of hard drive 1.
            Must be in the "profile" folder.
        promPath
            the path of the PROM (basically the BIOS) to load.
            If you are messing around with different versions of the BIOS oyu can set this
        startPaused (default is 1)
            Start the emulator paused, 0 will start the CPU immediately.
        ramInstalled (default is 16777216)
            RAM installed in bytes.
            Must be between 0 (in which case only the serial PROM console will be available) and 33534432 / 32MB (anything above 16MB was never available as an 
            official config, but seems to work).
        numBitplanes (default is 32)
            The number of bitplanes (one BP3 board is 4 bitplanes) to install. Affects maximum graphics bit depth and available graphics modes.
            Must be a multiple of 4 and between 4 and 32. There is no real reason to change this since oyu can use mapped modes with more bitplanes.
        defaultSwitchValue (default is 0x1F / 31, or autoboot from DSD 5217 disk controller, hard drive)
            A hexadecimal number indicating (see docs/boards/ip2.md) the default DIP switch value to use
        fakeGF2 (default is 0)
            Sets up a fake GF2 board which prevents bus errors from occurring so the graphical PROM monitor works. If this space does not bus error the GF2
            is assumed to be installed.
            NOTE: no attempt is made to emulate any aspect of the gf2 so GL2 will come up in serial mode with "fbc: dead meat" logged to the console.

Notes:
    "Unmapped write" or "Unmapped read" warnings: Ignore them, they are fine.
    "Bus Error" warnings: These are fine also unless the system obviously hangs or crashes. Bus errors are required to test for devices if those devices do not exist.

    If the PROM spits out a screen of the type:

        Fault Information (vector offset: xxxx):
        Exception: xxxxxxxxxxxxxxxxxx (Vector #xx)

        Processor Registers (ssp: xxxxxxxx):
            pc: xxxxxxxx  sr: xxxx

        Board Registers:
            text (base/limit): xxxx/xxxx
            stk  (base/limit): xxxx/xxxx
            status: xxxx parctl: xx mbp: xx

    Then it crashed. Unless you were using the edit memory commands this should never happen.
    If you did not intend to crash the machine and weren't trying to edit memory, take a screenshot of the screen and send it to me. 

    UNIX panic advice: 
        Probably it was a bug, either in GL2 or your 
        
Not done:
    - Reconfigurable machines
    - Debugger commands
    - Configuration
    - Most things emulation wise

3. BOOTING & USING UNIX

You can boot unix on the default boot options but it will take a very long time as the IRIS will try all possible(!) HDD boot options, and the controller we emulate is the last one it tries.
    - By default the system is set to AUTO BOOT mode and to boot from the "MD" (DSD 5217 / MiDas) disk controller. This will cause the IRIS to boot using our supported DSD 5217 disk controller. A 60 MB image of GL2 W3.6 can be provided if you ask me.
    To change the disk that it loads (backups HIGHLY recommended) set profileDisk0Path <blah>; the value is a file path within the profile folder.
        - If you want to mess around in the PROM, turn off autoboot mode in the Backpanel Switches section.
        - Other disk controllers include the Interphase Storager-II ESDI controller (support for which is not in mainline yet) and its predecessor the Interphase 2190.
        - On the real hardware you can also boot from XNS or TFTP via Ethernet, a floppy disk (on some models) EPROM board (for PROM testing) or just not boot at all (force PROM monitor regardless of autoboot settings). None of these are supported yet and some probably won't ever be.
        You can't create your own disk images yet, as the tape drive is not emulated. You have to install from QIC tape.
    - I recommend going to Backpanel Switches > Boot Mode and selecting the "[DSD 5217] HDD (MDx)" option.
    - Alternatively you can go to the PROM prompt and type "b md0:". Then the IRIS will try to boot it. If you want you can provide a file name but the IRIS
    will look for "defaultboot" on its own which will initialise vmunix.
    - The unix these machines run is called SGI UNIX System V, GL1 or GL2 depending on the version. Basically it's SysV0 with parts of SysV3.2, 4.2BSD (e.g. demand paging) and early SunOS, with custom SGI stuff like the
    graphics library (GL1 / GL2) and "extent file system" (XFS predecessor). There is a basic windowing system provided, called "mex". Most likely it will not work yet, because the graphics system is insufficiently emulated to do so.
    - The only editors built-in are vi (the "original" vi version 3.7) and ed; apparently EMACS was an option, but no such option was ever preserved. Let me know if you have a tape... 
    - Probably you could get ethernet and TCP/IP going if the EXOS201 is ever emulated. It supports 10 Mbit ethernet as well as the proto-Ethernet (1.0/2.0)


When you boot from the drive, GL2 prints its sign-on message, probes all supported devices, and runs init/rc like any Unix. (you can tell by the fact that you will be kicked to a "#" prompt), you will be kicked into SINGLE USER mode by default and the /usr folder WILL NOT BE MOUNTED, as it was restored from a separate tape. In order to mount the usr folder in single user mode, run "mount /dev/md0c /usr".  In order to get into multiuser mode, run "multi" or "telinit 3". On the first run the system will ask you what the model number of your system is. Enter "3115" (as we have the DSD disk controller). It will then ask you to set the date (you can accept the default date of December 20, 1987 and it will be fine); you can set any date from 1970-2038 as it is a Unix system and ask you if you want to check filesystem integrity (a very good idea on this system). Then it will ask you to log in. No accounts have any passwords, users include:

* root
* demos
* mexdemos
* gifts
* tutorial

"root" is recommended as the rest are unprivileged.

When you are done working with the system take into account that an OS this old does *NOT* have a good ability to recover from file system corruptions. Hence, you *must* run the following commands when you are done using the system (privileged user required):
    - "cd /" (shutdown has to be run from the root directory)
    - "shutdown"

    You will see "SHUTDOWN PROGRAM" and then the current date. Wait until *ALL* disk activity stops before closing the emulator.
    The OS will tell you if the filesystem is corrupted when you turn it back on.

4. Other things you can do

If you have an "mkboot" tape you can run "b md0:mdfex" (other fex's won't work), which will throw you into a low-level hard drive configuration / formatter tool.
    Messing around with this may end poorly

Run the "set debug 1" command in the PROM and then boot. It will spew out a deranged amount of logging.

5. Resources