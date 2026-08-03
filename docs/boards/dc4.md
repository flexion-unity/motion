DC4 hardcoded to Multibus IO + 0x4000 = 0x50004000h

Bit 9 selects register.
    50004200h -> DC4 flags

    Colourmap RAM (for non-direct colour modes):
        24 MCM6168P45 SRAM chips for 48 KB. (4096 colours). stored as 24bpp. A maximum of 16 maps (which are 256 colours each). P
        Seemingly stored like this:
    50004400h -> DC4 red colourmap ram start (written 16 bits at a time)
    50004600h -> DC4 green colourmap ram start (written 16 bits at a time)
    50004800h -> DC4 blue colourmap ram start (written 16 bits at a time)

    * prototype DC2 boards in non-direct colour mode could only map 4 maps of 256 colours instead of dc3+ which had either a single map of 4096 colours or 16 maps of 256 colours
