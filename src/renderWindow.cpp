#include "renderWindow.h"
#include "tools.h"

#include "SDL.h"
#include "SDL_image.h"

#include <iostream>
#include <string_view>

namespace
{
    /**
     * @brief Starts up SDL.
     * @return true if success.
     */
    bool initSDL()
    {
        // Initialization flag
        bool success { true };

        // Initialize SDL
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: "
                      << SDL_GetError() << std::endl;
            success = false;
        }

        // Initialize SDL image
        if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)))
        {
            std::cout << "IMG_init has failed. Error: " << SDL_GetError()
                      << std::endl;
            success = false;
        }

        return success;
    }
} // namespace

RenderWindow::RenderWindow(int width, int height, std::string_view title)
{
    if (initSDL())
    {
        // Create window
        window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, width, height,
                                  SDL_WINDOW_SHOWN);
        if (window)
        {
            // Create renderer for window
            renderer = SDL_CreateRenderer(window, -1,
                                          SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
            if (renderer == nullptr)
            {
                std::cout << "Unable to create rendering context. Error: "
                          << SDL_GetError() << std::endl;
            }
        }
        else
        {
            std::cout << "Window failed to init. Error: " << SDL_GetError()
                      << std::endl;
        }
    }
}

void RenderWindow::clear(const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

SDL_Texture* RenderWindow::loadTexture(std::string_view path)
{
    SDL_Texture* texture { IMG_LoadTexture(renderer, path.data()) };

    if (texture == nullptr)
    {
        std::cout << "Failed to load texture. Error: " << SDL_GetError()
                  << std::endl;
    }

    return texture;
}

void RenderWindow::render(SDL_Texture* texture, int posX, int posY,
                          const SDL_Color* tint)
{
    render(texture, { posX, posY }, 0, nullptr, { 1, 1 }, tint);
}

void RenderWindow::render(SDL_Texture* texture, const SDL_Point& position,
                          const SDL_Color* tint)
{
    render(texture, position, 0, nullptr, { 1, 1 }, tint);
}

void RenderWindow::render(SDL_Texture* texture, const SDL_Point& position,
                          double angle, const SDL_Point* center,
                          const SDL_Point& scale, const SDL_Color* tint)
{
    SDL_Point textureSize { tools::getSize(texture) };
    SDL_Rect source { 0, 0, textureSize.x, textureSize.y };
    SDL_Rect dest { position.x, position.y, textureSize.x * scale.x,
                    textureSize.y * scale.y };

    render(texture, &source, &dest, angle, center, tint);
}

void RenderWindow::render(SDL_Texture* texture, const SDL_Rect* source,
                          const SDL_Point& position, const SDL_Color* tint)
{
    SDL_Rect dest { position.x, position.y, source->w, source->h };
    SDL_Point origin { 0, 0 };

    render(texture, source, &dest, 0.0f, &origin, tint);
}

// TODO: Maybe add a flip parameter?
void RenderWindow::render(SDL_Texture* texture, const SDL_Rect* source,
                          const SDL_Rect* dest, double angle,
                          const SDL_Point* center, const SDL_Color* tint)
{
    // Check if texture is valid
    if (SDL_QueryTexture(texture, nullptr, nullptr, nullptr, nullptr) == 0)
    {
        if (tint)
            // Apply tint to texture
            SDL_SetTextureColorMod(texture, tint->r, tint->g, tint->b);
        // NOTE: Texture will not flip.
        // TODO: Test if angle is not null.
        SDL_RenderCopyEx(renderer, texture, source, dest, angle, center,
                         SDL_FLIP_NONE);
    }
}

void RenderWindow::display() { SDL_RenderPresent(renderer); }

void RenderWindow::close()
{
    // Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}
