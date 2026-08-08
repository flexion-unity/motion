/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    render_sdl3_passes.cpp: Implements all the SDL 3 rednerer's render passes 
*/


#include <render/render.hpp>
#include <render/sdl3/render_sdl3.hpp>
#include <render/sdl3/render_sdl3_passes.hpp>

namespace Motion
{
    /// @brief IT'S NOT PORTABLE!
    /// This basically implements a very basic SDL_GPU copy pass that does nothing other than upload a single texture representing the ntire screen
    void MainRenderPass::Render(Renderer* renderer, RenderTexture* texture)
    {
        // this rendering pass is onyl for SDL3
        RenderTextureSDL3* sdl3RenderTexture = static_cast<RenderTextureSDL3*>(texture);
        RendererSDL3* sdl3Renderer = static_cast<RendererSDL3*>(renderer);
        SDL_GPUTransferBuffer* transfer = sdl3Renderer->GetGPUTransferBuffer();
        SDL_GPUCommandBuffer* commandBuffer = sdl3Renderer->GetCommandBuffer();
        SDL_GPUTexture* swapchainTexture = sdl3Renderer->GetSwapchainTexture();

        uint32_t* pixels = (uint32_t*)SDL_MapGPUTransferBuffer(
            sdl3Renderer->GetGPUDevice(), 
            transfer,
            true); 

        if (!pixels)
            Logger::Log("MainRenderPass::Render - Failed to map GPU transfer buffer");

        // better hope they're the same size
        memcpy(pixels, texture->GetPixels(), texture->GetMemorySize());
        SDL_UnmapGPUTransferBuffer(sdl3Renderer->GetGPUDevice(), transfer);

        // send the commands
        SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer);

        // ok
        SDL_GPUTextureTransferInfo transferInfo = SDL_GPUTextureTransferInfo();
        transferInfo.pixels_per_row = texture->sizeX; 
        transferInfo.rows_per_layer = texture->sizeY;
        transferInfo.offset = 0;
        transferInfo.transfer_buffer = transfer;

        // similar to sdl_rect in old days it seems
        SDL_GPUTextureRegion rect = SDL_GPUTextureRegion();
        rect.texture = sdl3RenderTexture->GetRawTexture();
        rect.w = texture->sizeX;
        rect.h = texture->sizeY;
        rect.d = 1;

        SDL_UploadToGPUTexture(copyPass, &transferInfo, &rect, true);
        SDL_EndGPUCopyPass(copyPass);

        uint32_t realWindowSizeX = 0, realWindowSizeY = 0; 

        // this is an easy shit which means we don't need to run an entire manual graphics pipeline with pointless vertex and fragment shaders that do nothing
        // except render ocords of one specific texture. Woo!
        SDL_GPUBlitInfo blitInfo = SDL_GPUBlitInfo();

        blitInfo.source.texture = sdl3RenderTexture->GetRawTexture();
        blitInfo.destination.texture = swapchainTexture;

        blitInfo.source.x = blitInfo.destination.x = 0;
        blitInfo.source.y = blitInfo.destination.y = 0;

        switch (texture->drawType)
        {
            case RenderTextureDrawType::DrawAsWindowSize:
                blitInfo.source.w = renderer->GetWindow().GetWindowSizeX();
                blitInfo.source.h = renderer->GetWindow().GetWindowSizeY();
                blitInfo.destination.w = renderer->GetWindow().GetWindowSizeX();
                blitInfo.destination.h = renderer->GetWindow().GetWindowSizeY();
                break; 
            case RenderTextureDrawType::Default:
                blitInfo.source.w = blitInfo.destination.w = texture->sizeX;
                blitInfo.source.h = blitInfo.destination.h = texture->sizeY;
                break;
            case RenderTextureDrawType::Scaled:
                blitInfo.source.w = texture->srcSizeX;
                blitInfo.source.h = texture->srcSizeY;
                blitInfo.destination.w = texture->destSizeX;
                blitInfo.destination.h = texture->destSizeY;
                break;
        }

        blitInfo.clear_color = (SDL_FColor)(0.0, 0.0, 0.0, 1.0);
        blitInfo.load_op = SDL_GPULoadOp::SDL_GPU_LOADOP_LOAD;

        // bing bong
        SDL_BlitGPUTexture(commandBuffer, &blitInfo);

    }
};