/* 
    m  o  t  i  o  n
    The SGI Emulator

    Copyright (c)2026 starfrost

    render.hpp: Backend independent render stuff
*/

#pragma once
#include <Motion.hpp>

namespace Motion
{
    #define RENDER_LOG_PREFIX                   "Render - Core"
    #define DEFAULT_TEXTURE_BYTES_PER_PIXEL     4                   // Not sure why we would need anything else...

    // forward declare
    class Renderer;
    
    // a colour in rgba format like sgi uses
    // use imvec4 if ui code
    struct Color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a; 

        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a; 
        }

        Color() : Color(0, 0, 0, 0) { };
    }; 


    // base class for render texture. this is so we could use something other than SDL in the future.
    class RenderTexture
    {
    public: 
        RenderTexture(Renderer* renderer, int32_t sizeX, int32_t sizeY) 
        { 
            this->renderer = renderer; 
            this->sizeX = sizeX;
            this->sizeY = sizeY;
            this->stride = sizeX * sizeY * DEFAULT_TEXTURE_BYTES_PER_PIXEL;
            this->pixels = new uint8_t[sizeX * sizeY * DEFAULT_TEXTURE_BYTES_PER_PIXEL];
        };
        
        uint32_t sizeX = 0, sizeY = 0, stride = 0; 

        uint32_t GetPixel(int32_t x, int32_t y, Color color);
        Color GetPixel(int32_t x, int32_t y);
        void SetPixel(int32_t x, int32_t y, Color color); 
        void SetPixel(int32_t x, int32_t y, uint32_t color);

    protected:
        Renderer* renderer; 
        uint8_t* pixels;
        // this is a mirror texture that gets uploaded to the
    }; 

    /// @brief this class defines a render pass so that we can do e.g. GF2->UC4->DC4
    class RenderPass
    {
    public: 
        virtual void Render(RenderTexture* screen) { };
        const char* GetName() { return name; };

        // constructors

        RenderPass(const char* name)
        {
            strncpy(this->name, name, STRING_MAX_SHORT);
        }

        // have a parameterless constructor for e.g. standard arrays
        RenderPass() : RenderPass("Unnamed Render Pass (Thanks, bozo programmer)") { };

    private: 
        // for debug
        char name[STRING_MAX_SHORT] = {0};
    }; 

    /// @brief Base renderer class. Other renderers inherit from this
    class Renderer
    {
        // THis is the iris 3130's screen resolution. DON'T HARDCODE IT!
        #define WINDOW_SIZE_X       1024
        #define WINDOW_SIZE_Y       768

    public:
        virtual void Init() { };

        /// @brief Pumps the event queue for this renderer. Main Emulator and UI can be rendered before this.
        virtual void FramePreRender() { };

        /// @brief Performs the actual rendering.
        virtual void FramePostRender() { };
        virtual void Shutdown() { };

        // Getters for private fields
        int32_t GetWindowSizeX() { return windowSizeX; }; 
        int32_t GetWindowSizeY() { return windowSizeY; }; 

        // Setters for private fields
        virtual void SetWindowSize(int32_t x, int32_t y) { };
        
        void AddRenderPass(RenderPass& pass)
        {
            Logger::Log(RENDER_LOG_PREFIX, std::format("Added render pass: {}", pass.GetName()).c_str(), LogChannels::Debug);
            passes.push_back(pass);
        }

    protected: 
        int32_t windowSizeX = WINDOW_SIZE_X, windowSizeY = WINDOW_SIZE_Y;
        // vecotr of render passes
        std::vector<RenderPass> passes = std::vector<RenderPass>(); 
        RenderTexture* screen = nullptr;

    };


}