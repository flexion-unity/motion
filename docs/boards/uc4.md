UC4 is hardcoded to Multibus IO + 0x3000 = 0x50003000h

Uses a similar command system to the GF2 doard.

0x50003080h: Write to one of 17 buffers.
0x50003180h: Configure UC4 board multibus state
0x50003200h: Execute a command.

Actually a lot of UC4 commands are just never used. 

Anything else: Execute a DMA command (offset = (command ID << 1))
                well this is definitely an intriguing i/o scheme
                NEVER GETS CALLED BY ANY SOFTWARE AT ALL! BRILLIANT!