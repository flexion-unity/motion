/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    render_core.hpp: Backend independent render stuff. Currently just setpixel/getpixel though.
    HARDCODED TO 32BPP
*/

#include <render/render.hpp>

namespace Motion
{
    uint32_t RenderTexture::GetPixel(int32_t x, int32_t y, Color color)
    {
        uint64_t pixelIndex = (y * this->stride) + (x << 2);
        // checks might be too slow, this is hot-path stuff, so assert
        MOTION_ASSERT(pixelIndex >= ((sizeX * sizeY) << 2), "***** RenderTexture::GetPixel - INVALID pixel index write *****");

        // little endian
        return (pixels[pixelIndex + 3] << 24)
        | (pixels[pixelIndex + 2] << 16)
        | (pixels[pixelIndex + 1] << 8)
        | (pixels[pixelIndex]);
    }

    Color RenderTexture::GetPixel(int32_t x, int32_t y) 
    {
        uint64_t pixelIndex = (y * this->stride) + (x << 2);
        // checks might be too slow, this is hot-path stuff, so assert
        MOTION_ASSERT(pixelIndex >= ((sizeX * sizeY) << 2), "***** RenderTexture::GetPixel - INVALID pixel index write *****");

        // little endian
        return Color(pixels[pixelIndex], pixels[pixelIndex + 1], pixels[pixelIndex + 2], pixels[pixelIndex + 3]);
    }

    void RenderTexture::SetPixel(int32_t x, int32_t y, Color color)
    {
        uint64_t pixelIndex = (y * this->stride) + (x << 2);
        // checks might be too slow, this is hot-path stuff, so assert
        MOTION_ASSERT(pixelIndex >= ((sizeX * sizeY) << 2), "***** RenderTexture::SetPixel - INVALID pixel index write *****");

        pixels[pixelIndex] = color.r;
        pixels[pixelIndex + 1] = color.g;
        pixels[pixelIndex + 2] = color.b;
        pixels[pixelIndex + 3] = color.a;
    }

    void RenderTexture::SetPixel(int32_t x, int32_t y, uint32_t color)
    {
        uint64_t pixelIndex = (y * this->stride) + (x << 2);
        // checks might be too slow, this is hot-path stuff, so assert
        MOTION_ASSERT(pixelIndex >= ((sizeX * sizeY) << 2), "***** RenderTexture::SetPixel - INVALID pixel index write *****");

        pixels[pixelIndex] = color & 0xFF;
        pixels[pixelIndex + 1] = (color >> 8) & 0xFF;
        pixels[pixelIndex + 2] = (color >> 16) & 0xFF;
        pixels[pixelIndex + 3] = (color >> 24) & 0xFF;
    }

};