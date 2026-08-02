# Motion Changelog

Similar to a .plan file

## 2026-07-28

* added kernel and stack segment memory mappings
    * will be tied into mmu MemorySegment system
* release v0.0728 

## 2026-07-29

* implemented basic mmu address translation
    * ComponentMMU, has a translate method that returns a bool if a bus error occurred and takes a uint32_t* with a pointer to a final address to fill in.
        * I could just return a tuple. But this is critical path stuff.
        * Enough for the PROM to detect that we have 16 MB, rather than 0 or 1 MB.
        * Likely required also to run UNIX.
        * Mame was wrong - 13 bits instead of 10. Not sure why it works? Maybe I am wrong or maybe he only tested with limited ram
        * also has a start and shutdown method to measure tearing down and setting up the mmu in the addrspace system.
* guy wanted to add github actions support
    * he only did macos and linux. no windows. i can add windows later.
* added pagetable debug window
    * it sucks

## 2026-07-30

* this emulator hasn't been called iris in forever and now people know about it so change the namespace to motion and rename Iris.hpp to Motion.hpp.
* get rid of catchpoints due to UI space. instead we will have an option that allows to break on exception and to allow you to specify a simple ID to break on
* incremented coherent version to v0.6 (v0.5 = v0.0728 verison)
    * refactored the totally awful coherent file structure
        * coherentsystem now in coherent_system.cpp
    * added stack window and options menu
* made the event system work
    * don't like it since it calls empty methods. but having io device specific interface seemed too brittle.
* keyboard now correctly configures itself
    * however this makes the PROM not work.

## 2026-07-31

* nothing really

## 2026-08-01

* implemented key and mouse events after figuring out the keyboard only translates keys. doh!
    * need to translate ASCII to SGI
* translating SDL keycodes to SGI and sending them along on key is enough to get to a loop. it seems like it is trying to do a lot of i/o so maybe it is in graphics mode
    * PROM explodes and enters serial mode if you press a key too quickly during boot, so just odn't allow keypresses until keyboard init
    * typing PROM commands proves that its the PROM console, and that it works. cool!
* started multibus

## 2026-08-02

* wrote the first pass of the rendering api
    * everything is a render pass
    * backend-independent textures, inheriting from rendertexture 
        * sdl gpu textures are pushed to the gpu at the end of each frame. this should be in its own thread like iris
    *  since a uint8_t* of pixels is renderer independent and we will just be pushing to the gpu, make rendertexture nonvirtual for now. We can always change it later (woo, oop). it would likely be very slow to run a check all the time, so implement asserts for hotpath sanity checks (i.e. should never happen on a release build of the emulator.)
    * Implemented a basic renderer that has a single gpu copy pass that uploads a screen image to the screen as modified by any number of previous render passes. It shares  command buffer and swapchain texture with imgui
