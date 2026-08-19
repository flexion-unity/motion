/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    render_sdl3.hpp: Defines interface to SDL3 + IMGUI SDL3GPU renderer
*/

#pragma once
#include <Motion.hpp>
#include <render/render.hpp>

#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

namespace Motion
{
    #define LOG_PREFIX_RENDER_SDL3          "Render - SDL3"
    #define WINDOW_TITLE_DEFAULT            APP_NAME " " APP_VERSION " - SGI Emulator (c) 2026 Starfrost"

    class RenderTextureSDL3 : public RenderTexture
    {
    public:
        // THIS ALLOCATES THE TEXTURE RIGHT THERE!
        RenderTextureSDL3(Renderer* renderer, uint32_t sizeX, uint32_t sizeY, RenderTextureDrawType drawType  = RenderTextureDrawType::Default);
        RenderTextureSDL3(Renderer* renderer, int32_t sizeX, int32_t sizeY, int32_t srcSizeX, int32_t srcSizeY, int32_t destSizeX, int32_t destSizeY, 
            RenderTextureDrawType drawType = RenderTextureDrawType::Default);
        ~RenderTextureSDL3();

        // getters for privates
        SDL_GPUTexture* GetRawTexture() { return texture; }; 

    private:
        SDL_GPUTexture* texture; 
        Renderer* renderer;
    };

    class WindowSDL3 : public Window
    {
    public: 
        void Start() override; 
        void Shutdown() override; 

        void SetWindowSize(int32_t x, int32_t y) override;
        
        SDL_Window* GetInternalWindow() { return window; };
    private:
        SDL_Window* window; 
    };

    /// @brief Base renderer class. Other renderers inherit from this
    class RendererSDL3 : public Renderer
    {
        friend class RenderTextureSDL3;
    public:
        
        void Init() override;
        void FramePreRender() override;
        void FramePostRender() override;
        void Shutdown() override;

        /// @brief set the screen size
        /// @param x the x coordinate of the screen size to set
        /// @param y the y coordinate of the screen size to set
        void SetScreenSize(int32_t x, int32_t y) override;

        // Getters for private fields
        SDL_GPUDevice* GetGPUDevice() { return gpuDevice; };
        SDL_GPUTransferBuffer* GetGPUTransferBuffer() { return transfer; };
        WindowSDL3& GetWindow() override { return window; };
        SDL_Window* GetInternalSDL3Window() { return window.GetInternalWindow(); };

        SDL_GPUCommandBuffer* GetCommandBuffer() { return commandBuffer; };
        SDL_GPUTexture* GetSwapchainTexture() { return swapchainTexture; };

        // Setters for private fields

    private:
        WindowSDL3 window;

        SDL_GPUDevice* gpuDevice;
        
        /// @brief This is a GPURenderer, for convenience (i want to ues SDL_Texture, but IMGUI SDLGPU3 is much better than Renderer3)
        SDL_Renderer* renderer;  

        /// @brief the gpu transfer buffer. cached for performance reasons. shared by multiple passes.
        SDL_GPUTransferBuffer* transfer; 

        // **** PER FRAME STUFF ****
        SDL_GPUCommandBuffer* commandBuffer;
        SDL_GPUTexture* swapchainTexture;

        /// @brief imgui io
        ImGuiIO* io;
        
        void CreateTransferBuffer();

        /// @brief draw a simple initial display
        void DrawInitialDisplay();
    };
}