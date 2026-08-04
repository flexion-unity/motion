/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    virtualfb.hpp: a virtual framebuffer
    this allows multiple components to control a framebuffer that is different to the window framebuffer

    Juniper machine graphics render to 1024x1024 display but only 1024x768 is shown. So we do this and copy 1024x768 out later.
    Eventually this could be extended for e.g. Clover2 tiled rendering.

    ANd yes it's a damn staatic class AGain!
*/

#include <Motion.hpp>
#include <render/render.hpp>

namespace Motion
{
    enum VirtualFramebufferType
    {

    }; 
    
    class VirtualFramebuffer
    {
    public:

    private: 

    }; 
}; 