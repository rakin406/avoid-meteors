#include "renderWindow.h"
#include "tools.h"

#include "SDL.h"
#include "SDL_image.h"

#include <iostream>
#include <string_view>

RenderWindow::RenderWindow(int width, int height, std::string_view title)
{
    // Attempt to create a window
    window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == nullptr)
    {
        std::cout << "Window failed to init. Error: " << SDL_GetError()
                  << std::endl;
    }

    // Attempt to create rendering context
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        std::cout << "Unable to create rendering context. Error: "
                  << SDL_GetError() << std::endl;
    }
}

void RenderWindow::clear(const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

SDL_Texture* RenderWindow::loadTexture(std::string_view fileName)
{
    SDL_Texture* texture { IMG_LoadTexture(renderer, fileName.data()) };

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
    renderEx(texture, { posX, posY }, 0, 1, tint);
}

void RenderWindow::renderV(SDL_Texture* texture, const SDL_Point& position,
                           const SDL_Color* tint)
{
    renderEx(texture, position, 0, 1, tint);
}

void RenderWindow::renderEx(SDL_Texture* texture, const SDL_Point& position,
                            double rotation, int scale, const SDL_Color* tint)
{
    SDL_Point textureSize { tools::getSize(texture) };
    SDL_Rect source { 0, 0, textureSize.x, textureSize.y };
    SDL_Rect dest { position.x, position.y, textureSize.x * scale,
                    textureSize.y * scale };
    SDL_Point origin { 0, 0 };

    renderPro(texture, &source, &dest, rotation, &origin, tint);
}

void RenderWindow::renderRec(SDL_Texture* texture, SDL_Rect* source,
                             const SDL_Point& position, const SDL_Color* tint)
{
    SDL_Rect dest { position.x, position.y, source->w, source->h };
    SDL_Point origin { 0, 0 };

    renderPro(texture, source, &dest, 0.0f, &origin, tint);
}

// TODO: Maybe add a flip parameter?
void RenderWindow::renderPro(SDL_Texture* texture, const SDL_Rect* source,
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
        SDL_RenderCopyEx(renderer, texture, source, dest, angle, center,
                         SDL_FLIP_NONE);
    }
}

void RenderWindow::display() { SDL_RenderPresent(renderer); }

void RenderWindow::close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}