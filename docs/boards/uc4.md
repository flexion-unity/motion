UC4 is hardcoded to Multibus IO + 0x3000 = 0x50003000h

Uses a similar command system to the GF2 doard.

0x50003080h: Write to one of 17 buffers.
0x50003180h: Configure UC4 board multibus state
0x50003200h: Execute a command.

Actually a lot of UC4 commands are just never used. 

Anything else: Execute a DMA command (offset = (command ID << 1))
                well this is definitely an intriguing i/o scheme
                NEVER GETS CALLED BY ANY SOFTWARE AT ALL! BRILLIANT!

So the UC4 is programmed by setting up its buffers, and then executing a command by writing its ID to 50003200h.
Basically the buffers are 17 special purpose registers and the commands perform actions based on those registers.

Note: this is only for GF2/"Enhanced IRIS" boards. The UC3 used in the GF1 is a bit different and can only be written via the BPC from the GF1 board side. It can be logically considered to be a part of the FBC. As a side effect there are no buffers, you have to write every single parameter in order every time you execute a command, wasting I/O speed.
