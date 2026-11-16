# GF2

The GF2 board (**G**raphics and **F**ramebuffer 2) is the GL2/IP2 generation 3D graphics board.
There's a lot of logic on the GF2 but it really boils down to 3 parts.

* GE - Geometry Engine
* FBC - Frame Buffer Controller
* BPC - Bit Plane Controller

## Geometry Engine

The highest spec Juniper (IRIS 3000) series graphics option basically has 14 chips.

Two are GAs which FIFO the data coming in over the local bus and convert from IEEE 754 to a custom 20.8 FP format.
These are at the start and end of the chain.

A Geometry Engine is four 32-bit ALUs hooked up to a microcode store and a configuration register. The microcode store stores all possible operations and the configuration engine determines which part of the U/code runs

10MHz GEs use GE 2.5. Older versions are likely GE 2.0.

The rest of the GEs do this in order:

Matrix Multiplication. 1 chip does one row of the matrix. Chaining four together creates a 4x4 matrix multiplier. Cool eh?
In fact this can probably do one row per cycle. Later on, in Clover 1 (4D/60, 4D/70G etc) they doubled these up to run two matrix multiplications in parallel.

Clipping. There's six of these, which do the six clipping directions (left, right, top, bottom, near, far). Also, one of these
seems to be used for Z-buffering on some machines.

Scaling. One GE does two coordinates. 
    ONe does X/Y
    One does Z. Note there are two ALUs left. Jim Clark wrote about introducing stereoscopic 3D rendering using the other two.

Current best guess (based on a GF2 board that had all its heatsinks pulled & the 1982 paper)

```
GA In -> MatM X -> MatM Y -> MatM Z -> MatM W -> Z / ClipLeft?
                                                                          |
                                                                          |
Scale XY <- ClipFar <- ZOut /ClipNear <- ClipBottom <- ClipTop <- ClipRight 
|
|
--> ScaleZ -> Out
```

## Frame Buffer Controller

Four hopelessly bottlenecked AMD Am2903 bit-slice processors combined with a sequencer (amd am2910). These perform all non wireframe operations and run custom SGI microcode.

4096 states, 4 words.

There is actually 4096 * 4 words of ucode = 32 kb.

Each slice is 16 bits. so each state is 64 bit

-->so each slice is one microword? or is each *state* one microword? 

There's 55 bits in each microword set out like so:

    Output NextAddress ={"di",		0,	15,	0};
    // 2910 Inputs 
    Output i0 = 		{"i0",		16,	16,	0};
    Output i1 =		    {"i1",		17,	20,	6};
    Output i2 =		    {"i2",		21,	24,	4};  deflt: WRE 
    
    Output cin =		{"cin",		25,	25,	0};
    Output earbar =		{"earbar",	26,	26,	0};
    Output ealbar =		{"ealbar",	27,	27,	0};
    Output addra =		{"addra",	28,	31,	0};
    Output addrb =		{"addrb",	32,	35,	0};

    Output clklong =	{"clklong",	36,	36,	0};     
    Output get = 		{"get",		37,	37,	0};
    Output put =		{"put",		38,	38,	0};
    Output loadout = 	{"loadout",	39,	39,	0};    
    Output enram = 		{"enram",	40,	40,	0};
    Output rdram =		{"rdram",	41,	41,	0};
    Output highbyte =	{"highbyte",	42,	42,	0};
    Output rightjust =	{"rightjust",	43,	43,	0};

    Output seqop =		{"seqop",	44,	47,	14};
    Output ccsel =		{"ccsel",	48,	50,	0};
    Output fbccode =	{"fbccode",	51,	54,	0};
    Output reverse =	{"reverse",	55,	55,	0};

    Output DIsrc = 		{"DIsrc",	0,	7,	0};
            pseudo-field for recording intended use of DI bus	
    Output seqtype = 	{"seqtype",	0,	7,	0};
            pseudo-field for categorizing 2910 opcode used	
    Output microconst = 	{"microconst",	-1,	-1,	0};
            pseudo-field for recording whether MICROCONST invoked 

I0 is 1 on SETSOP with qop==QOPERAND and set to 0 for MUL/DIV ops.
I1 is set to ALU Ops and set to 0 for MUL/DIV ops.
I2 is used for FTOYANDQ - figure this one out and set to 0 for MUL/DIV ops.
CIN is 1 if ALU OP, 0 otherwise
EALBAR is 1 if SETROP halves parameter is ALL16 or HI8, otherwise 0
EARBAR is 1 is SETROP halves parameter is ALL16 or LO8, otherwise 0
ADDRA is set to register to use in SETROP call (R-op)
ADDRB is set to register to use in SETSOP call (S-op)
CLKLONG is 1 in _MulDiv, _PropIn, 0 in _PropOut16, _PropOut12
GET means "get geometry engine data"
PUT = 0 is used to read from the BPC / UC4; PUT = 1 is used to write to the bpc / uc4; maybe
LOADOUT is 1 in _DOToOutreg; so it sets what to output based on  ???
ENRAM is 1 in RAMOP (called from SETSOP) commands=RAMRD,RAMWR
RDRAM is 1 in RAMOP (called from SETSOP) commands=RAMRD (so it means if we are reading ram)
HIGHBYTE is 1 in YTODO if y==HI8 (makes sense, we are writing the high byte!)
RIGHTJUST is 1 in _PropOut12, _PropIn, LOADDI MULTIBUS
SEQOP is set to op in SEQOP
CCSEL is sequencing procedure branch type (0-7)
fbccode is the BPC command. It can be used to control the uc4 as well as other stuff.
If command is <= 0x10 reverse is set to 0, otherwise 1.

PSEUDO FIELDS : 

DIsrc is LOADDI x parameter
seqtype is taken from SEQ Op parameter:
	case JZER:  Setfield(seqop,0); Setfield(seqtype,NONBRANCH); break;
	case JSUB:  Setfield(seqop,1); Setfield(seqtype,BRANCH); break;
	case JMAP:  Setfield(seqop,2); Setfield(seqtype,SPECIAL); break;
	case JUMP:  Setfield(seqop,3); Setfield(seqtype,BRANCH); break;
	case PUSH:  Setfield(seqop,4); Setfield(seqtype,COUNTER); break;
	case JSRP:  Setfield(seqop,5); Setfield(seqtype,BRANCH); break;
	case VECT:  Setfield(seqop,6); Setfield(seqtype,NONBRANCH); break;
	case CJRP:  Setfield(seqop,7); Setfield(seqtype,BRANCH); break;
	case LOUP:  Setfield(seqop,8); Setfield(seqtype,NONBRANCH); break;
	case RPCT:  Setfield(seqop,9); Setfield(seqtype,BRANCH); break;
	case RETN:  Setfield(seqop,10); Setfield(seqtype,NONBRANCH); break;
	case CJPP:  Setfield(seqop,11); Setfield(seqtype,SPECIAL); break;
	case LDCT:  Setfield(seqop,12); Setfield(seqtype,COUNTER); break;
	case TEST:  Setfield(seqop,13); Setfield(seqtype,NONBRANCH); break;
	case CONT:  Setfield(seqop,14); Setfield(seqtype,NONBRANCH); break;
	case THWB:  Setfield(seqop,15); Setfield(seqtype,BRANCH); break;

constproc : ????


## Bit Plane Controller
The interface to the UC4 board (in GF1 boards this is the only way to write to the UC4, but in GF2, Multibus I/O at 50003000-50003fff is exposed as the UC4 board and used by e.g. IP2 PROM. Emulation of this has already occurred) and VRAM. 