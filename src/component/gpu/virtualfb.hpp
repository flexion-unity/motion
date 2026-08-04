/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    virtualfb.hpp: a virtual framebuffer
    this allows multiple components to control a framebuffer that is different to the window framebuffer

    Juniper machine graphics render to 1024x1024 display but only 1024x768 is shown. So we do this and copy 1024x768 out later.
    Eventually this could be extended for e.g. Clover2 tiled rendering.

    ANd yes it's a damn staatic class AGain!

    Since this is a bitplane based virtual framebuffer, the calculation of the pixel positions etc is up to the GPU implmenting this
    FB.
*/

#include <Motion.hpp>
#include <render/render.hpp>

namespace Motion
{
    class VirtualFramebuffer
    {
    public:
        static void Create(int32_t x, int32_t y, int32_t numBitplanes);
        static void StartFrame();

        static size_t GetMemorySize() { return sizeX * sizeY * numBitplanes; };

        // Getters for private fields ( we wouldn't want a BOZO doing framebuffer = null; )
        static uint8_t* GetPixels() { return pixels; };
        static size_t GetSizeX() { return sizeX; };
        static size_t GetSizeY() { return sizeY; };
        static size_t GetNumBitplanes() { return numBitplanes; };

    private: 
        inline static uint8_t* pixels;

        inline static size_t sizeX;
        inline static size_t sizeY;
        inline static size_t numBitplanes;
    }; 

    /// @brief a render pass that copies the physical framebuffer to the buffer screen
    /// op
    class CopyVirtualToPhysicalFbRenderPass : RenderPass
    {
        void Render(Renderer* renderer, RenderTexture* screen) override; 
    }; 

}; 