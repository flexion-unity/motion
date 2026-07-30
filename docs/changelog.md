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