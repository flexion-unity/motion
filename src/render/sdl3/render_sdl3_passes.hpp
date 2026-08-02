/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    render_sdl3_passes.hpp: Defines all the SDL 3 rednerer's render passes 
*/

#include <render/render.hpp>
#include <render/sdl3/render_sdl3.hpp>

namespace Motion
{
    class MainRenderPass : public RenderPass
    {
    public:
        void Render(Renderer* renderer, RenderTexture* texture) override;
        MainRenderPass() : RenderPass("Main - Push Raw SGI GPU Data to Host GPU") { };

    private: 

    };
}; 