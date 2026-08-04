IRIS PROM Boot process (PROM Version 3.0.11):

"evec.h" [gl2/stand/mon/evec.h]

1. CPU board IP2 comes up.
2. Ip2 Memory map by default:
    See ip2map.md
    
"locore.c" [gl2/stand/mon/locore.c] - actually an asm file. Huh!

start: 

Boot Phase 0 (pre-LEDs)

3. Store system initialisation level in D1.
   Store 0 in D2.

4. Enter supervisor mode and set interrupt priority to 7
5. Reprogram the interrupt vector base register to point at 30000000h PROM text
6. Turn off cache (CACR <- 0)
7. For DUART 0: 
    Write 0xEB to register 04h (04h on write is Aux. Control Register):
        - BRG set 2
        - Timer mode
        - Crystal or external clock (not divided)
        - Enable change of state interrupts for IP3, IP1, IP0
    Set counter refresh rate to 14
    Set OPCR (DUART register 13h, write) to 0xF4
        - OP7 pin -> Channel B tranmitter interrupt output (complement of TxRDYB, not masked, open-drain)
        - OP6 pin -> Channel B transmitter interrupt output (complement of TxRDYA, not masked, open-drain)
        - OP5 pin -> Channel A transmitter interrupt output (complement of ISR[5], not masked, open-drain)
        - OP4 pin -> Channel A receiver interrupt output (complement of ISR[1], not masked, open-drain)
        - OP3 pin -> counter/timer output
        - OP2 pin -> complement of OPR[2]
8. Determine board revision by reading bit 5 of mouse button register
9. If Rev A, skip to step 11 (step 10 is done later, by Unix.)
10. Write same values as above to DUART 1, then skip to step 12.
11. Configure "master or slave" status (which determines if the pipe and DUARTs are driven by this machine or not. Presumably for networking multiple machines together...)
    -> Set Status and Parity Control registers to 0
    -> If SW_MASTERSLV bit 4 is set, we are a slave, otherwise master.
    -> Set debug LEDs to 0

    If we are a slave:
        -> Allow Multibus access
        -> Disable "Pipe" and DUART accesses (Pipe is probably the internal bus between vram, gpu and cpu)
    
    If we are a master:
        -> Enable Pipe, DUARTs and Multibus


Boot phase 1 ("initdone"):

12. Disable user-mode access to MUltibus by setting Multibus Protect register to zero
13. Reset the initial stack pointer
14. Clear "private" part of 2 KB "Boot RAM" (as it's static RAM)
15. Increment initialisation level
16. Call init function

Boot phase 2 ("init(flag)") [gl2/stand/mon/init.c]

17. Checksum the 2kb IP2 SRAM common area between the PROM and GL2.
18. If it's fine, skip to step 27. otherwise, initialise the common area:
    19. Store the switch reg in SRAM.
    20. Check multibus amster or slave status. If master, reset multibus
    21. Spin (loop i = 0 to 150000) ("well, it seems to work!")
    22. Turn off multibus init.
    23. Spin (loop i = 0 to 10000)
    24. clear 256 bytes for power data etc.
    25. Determine how much memory is installed in the system. Do this by:
    - in reverse from 31 MB (0x1F00000-1FFFFFF) to 0 MB (0x0-0xFFFFF):
        - fill 1mb worth of pagetable entries (256)
        - set to access
        - set the first 256 virtual pages to access the physical pages for the current megabyte
            - write 0xDEAD, -1 (0xFFFF), 0xAAAA, 0x5555 and the current megabyte # to it
    - forward this time
        - check those patterns exist. if they do, increment the # of MB
            - also a basic memory test 

    26. Add 0x100000 to multibus io and mem address if slave.
    27. todraminit(): read the clock chip. reinitialise all clock data and DC4 configuration data if the clock is dead, otherwise, load it from the clock chip.
    28. compute and store the checksum
29. Clear TCP boot related data in the SRAM common area.
30. ttyconfig():
    DUART0 port A (keyboard) -> 600 baud
    DUART0 port B, DUART1 port A, DUART1 port B -> 300, 600, 1200, 9600, or 19200 baud based on back panel switch configuration settings. (3 bit number)
31. Spin (loop i = 0 to 50000)
32. Determine if the FP1 (Weitek 7034/7035 chips) board is installed.
    Turn on quick timeouts so it doesn't take too long,
    Set FPA_EREG (0x8) to 0x0, dummy opcode. (writes to 0xF0008800)
    If it bus error'd there is no FPA installed. Report it to UNIX later.


33. Remove previous init call from the stack (what?)
34. Set Debug LEDs to 1.
35. Call confidence tests

Boot phase 3 ("confid"): [gl2/stand/mon/locore.c]
    -> immediately RTS. Dummied out! 
    -> Looks like they were used (as well as 2, 3, 4, 5 in the LEDs) in the 68010-based PM2 board prom, but not in the IP2 board. Presumably 

36. Set debug LEDs to 6.
37. Test memory word alignment.
38. Zero 2kb private SRAM memory.
39. Test interrupt levels on 68020.

Boot phase 4 (after confid)
40. Turn on Cache (CACR->1) (we just assume it works here)
41. Determine if the 2 KB SRAM exists. If not, don't configure BS.
42. Allocate PROM BSS segment.
43. Turn on RAM parity checking on kernel (supervisor mode), user (user mode) and multibus I/O.
44. exit boot mode (STATUS_REG &= ~(ST_SYSSEG) in C)
45. Set Debug LEDs to 7.
46. call main function

Boot phase 5 (main): [gl2/stand/mon/main.c]

47. Check if the PROM is actually running in the PROM context or was just launched as a program (presumably an SGI testing function).
    If so -> skip to step 55.
48. Set Debug LEDs to 8.
49. Program text/data base to be at PTE 0, with no limit.
50. Program page table based on how many megabytes init() told us (via the common area) exist.
    They just literally translate physical to virtual here.
51. Zero out the nonexistent MB PTEs.
52. Point multibus to the last physical megabyte, if memory exists.
53. Program the PTEs for the BSS segment (again???)
54. Spend far too long building a bitmap of the memory that exists and where (32mb = 0xFFFFFFFF, 0mb = 0x00000000)
    maybe it helps finding where memory failed i guess
55. Set Debug LEDs to 9.
56. Set up multibus i/o and memory as viewed from multibus

Boot phase 6 (con_config): [gl2/stand/mon/init.c]

57. Try to reset the keyboard by sending 0x10 (CONFIG_REQUEST) to DUART0 Port A
58. If nothing comes back, the keyboard is not connected. Do not try to initialise the graphics system. Advance to phase 10.

59. The keyboard returned something. Woo. Now what happens depends on PROM version:

    (PROM Version >= 3.0.11)
    
    Use this to determine the keyboard type. Read two bytes.
        0xAA -> IRIS 3000-type (Keytronics) keyboard.
        0x6A 0x00 -> IRIS 4D/60 USA keyboard
        0x6A 0x01 -> IRIS 4D/60 ISO keyboard.
    
    Flash the keyboard LEDs. LED 0 is always on, every 16ms another of the 7 LEDs comes on, then the rest except 0 are turned off. 
    Beep the keyboard beeper.

    (PROM Version <= 3.0.10)

    IRIS 4D keyboard is not supported, so only 0xAA is returned.
    Flash the keyboard LEDs. 0 to 7, in a row, every 16ms. 0 stays on after.
    Don't beep in this version.

60. Since the keyboard initialised, try to initialise enough of the graphics hardware to display a graphical PROM display

Boot phase 7 (ScreenInit) [stand/lib/gl/scrinit.c]
Determine graphics type and initialise scren.

61. Initialise a micro-GL2. Call ScreenConfig (since glx.scrinit will be 0).
62. set DC4 config bits based on SRAM common area.
63. gl2_probe: Determine if the system is a GL1 (GF1+DC3+UC3) or GL2 (GF2+DC4+UC4) system. [stand/lib/gl/gl2gl.c]
    If the FBC flags dword exists (0x50002400, Multibus i/o) then the machine is GL2, otherwise GL1.
        glx.hwversion is 2 if gl2, 1 if gl1
    Set GLX scrinit function to gl1_scrinit or gl2_scrinit
64. configure_screen: 
    Set up DC4 configuration for the screen parameters. ("this code is really stupid") for rs170 (NTSC) or another type of screen
    
65. clear GL type in SAM
66. Set the screen size.   
67. if gl1, OR SRAM common area GL flag with 0x40 (GL2). otherwise 0x80 (gl1)
68. Clear state using GLX.
    * gl1_nostate / gl2_nostate

69. glinit [stand/lib/gl/scrinit.c]

70. Call GLX init function. Let's use the GL2 version
71. Initialise the UC4 board and FBC.
72. Initialise the Am2903 fbc:
    - does some crazy buggy stuff like writing the same register 4 times in a row and clearing the FBC interrupts 20 times with increasing delays. not great h/w.
    - comments indicate that they were struggling to get it to work consistently.

73. Initialise the UC4 board:
    - Enable UCRAddr bit 9
    - Set the UC4 to load configuration parameters.
    - Set the boundaries that the UC4 will draw within to the size of the screen discovered in step 64.
    - Set the UC4 screen mask to 0 (so don't mask anything.)
74. Load in the PROM's built in font data to the font RAM.
    - Set UC4 font base address to 0.
    - write every font character
75. Fill the screen with black (gl2_fill)
    - commented out code to draw a logo here!
76. Configure GLX putat, putline, fill, mapcolor, setmap and nostate function pointers.
77. Call glloadcolormap to load the colormap with 8 grayscale colours. Green on black.
78. round the screen boundaries to an integer number of lines and columns
79. move the cursor to 0,0
80. set GLX.inited to 1
81. create a window boundary to draw the PROM console within - 80 x 40 characters (640 * 320 pixels I think)

If ScreenInit returned 0, print "Graphics Hardware failed" advance to phase 10.
Otherwise advance to phase 8.

Boot Phase 8 (TermInit)

82. Initialise a terminal (as long as it is under 40 lines high)
    Allocate memory for text (sgi call their malloc "awful_alloc")
    Delete old text if we reinitialised the terminal

If TermInit(-1) returned 0, print "Graphics Hardware failed".

Boot phase 9 (Done)

83. Reassign common data character get/put functions to functions that use the "mini-GL2" in the PROM to write to the screen.

84. Print boot messages starting with "IRIS (IP2 - Revision %c) Monitor Version %s"
    - many of them are suppressed if the "quiet mode" switch is flipped
85. Kick the user into a console that they can use to interact with the machine and boot from a medium. 