#include "renderWindow.h"
#include "tools.h"

#include "SDL.h"
#include "SDL_image.h"
#include <SDL_ttf.h>

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

        // Initialize SDL_ttf
        if (TTF_Init() == -1)
        {
            std::cout << "SDL_ttf could not initialize. SDL_ttf Error: "
                      << TTF_GetError() << std::endl;
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

RenderWindow::~RenderWindow() { close(); }

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

SDL_Texture* RenderWindow::loadTexture(std::string_view textureText,
                                       const SDL_Color& textColor)
{
    // Render text surface
    SDL_Surface* textSurface { TTF_RenderText_Solid(font, textureText.data(),
                                                    textColor) };
    SDL_Texture* texture { nullptr };

    if (textSurface == nullptr)
    {
        std::cout << "Failed to load text surface. SDL_ttf Error: "
                  << TTF_GetError() << std::endl;
    }
    else
    {
        // Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);

        if (texture == nullptr)
        {
            std::cout << "Failed to create texture from rendered text. Error: "
                      << SDL_GetError() << std::endl;
        }

        // Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    return texture;
}

bool RenderWindow::loadFont(std::string_view path, int size)
{
    // Open the font
    font = TTF_OpenFont(path.data(), size);

    if (font == nullptr)
    {
        std::cout << "Failed to load lazy font. SDL_ttf Error: "
                  << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}

void RenderWindow::render(SDL_Texture* texture, float posX, float posY,
                          const SDL_Color* tint)
{
    render(texture, { posX, posY }, 0, nullptr, { 1.0f, 1.0f }, tint);
}

void RenderWindow::render(SDL_Texture* texture, const SDL_FPoint& position,
                          const SDL_Color* tint)
{
    render(texture, position, 0, nullptr, { 1.0f, 1.0f }, tint);
}

void RenderWindow::render(SDL_Texture* texture, const SDL_FPoint& position,
                          double angle, const SDL_FPoint* center,
                          const SDL_FPoint& scale, const SDL_Color* tint)
{
    SDL_FPoint textureSize { tools::getSize(texture) };
    SDL_Rect source { 0, 0, textureSize.x, textureSize.y };
    SDL_FRect dest { position.x, position.y, textureSize.x * scale.x,
                     textureSize.y * scale.y };

    render(texture, &source, &dest, angle, center, SDL_FLIP_NONE, tint);
}

void RenderWindow::render(SDL_Texture* texture, const SDL_Rect* source,
                          const SDL_FPoint& position, const SDL_Color* tint)
{
    SDL_FRect dest { position.x, position.y, source->w, source->h };
    SDL_FPoint origin { 0.0f, 0.0f };

    render(texture, source, &dest, 0.0f, &origin, SDL_FLIP_NONE, tint);
}

void RenderWindow::render(SDL_Texture* texture, const SDL_Rect* source,
                          const SDL_FRect* dest, double angle,
                          const SDL_FPoint* center,
                          const SDL_RendererFlip& flip, const SDL_Color* tint)
{
    // Check if texture is valid
    if (SDL_QueryTexture(texture, nullptr, nullptr, nullptr, nullptr) == 0)
    {
        // Apply tint to texture
        if (tint)
            SDL_SetTextureColorMod(texture, tint->r, tint->g, tint->b);
        SDL_RenderCopyExF(renderer, texture, source, dest, angle, center, flip);
    }
}

void RenderWindow::display() { SDL_RenderPresent(renderer); }

void RenderWindow::close()
{
    // Free global font
    TTF_CloseFont(font);
    font = nullptr;

    // Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;

    // Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
