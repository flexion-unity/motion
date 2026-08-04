/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    virtualfb_copy_pass.cpp : Copy pass
*/

#include <Motion.hpp>
#include <render/render.hpp>
#include <component/gpu/virtualfb.hpp>

namespace Motion
{
    void CopyVirtualToPhysicalFbRenderPass::Render(Renderer* renderer, RenderTexture* screen)
    {
        // i PRESUME that the fb is set up such that 
        // y=0...128    not visible
        // y=128...896  visible
        // y=896...1024 not visible
    }
}