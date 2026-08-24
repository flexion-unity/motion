# The Motion SGI Emulator

Currently targeting GL2 W3.6 & the IRIS 3130, since they have much more documentation than the less obscure machines, oddly.

* assets - assets (should ideally be optional)
* docs - documents
* external - submodules
* roms - ROMs, required for operation of the system
* src - our source

## How to build
1. git clone --recursive
2. (whatever you use cmake for)

Everything is statically linked.

If you have an error about missing SDL3, `git submodule update --init` will clone all submodules for you.
Tracks the latest release of SDL 3 (3.4.x) dev.