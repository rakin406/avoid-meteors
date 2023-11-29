#include "renderWindow.h"
#include "components.h"

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
                          const SDL_Color& tint)
{
    SDL_Rect src {};
    src.x = 0;
    src.y = 0;
    src.w;
    src.h;

    SDL_QueryTexture(texture, nullptr, nullptr, &src.w, &src.h);

    SDL_Rect dst {};
    dst.x = posX;
    dst.y = posY;
    dst.w = src.w;
    dst.h = src.h;

    SDL_SetTextureColorMod(texture, tint.r, tint.g, tint.b);
    SDL_RenderCopy(renderer, texture, &src, &dst);
}

void RenderWindow::renderV(SDL_Texture* texture, const Position& position,
                           const SDL_Color& tint)
{
    render(texture, position.x, position.y, tint);
}

void RenderWindow::renderEx(SDL_Texture* texture, const Position& position,
                            float rotation, float scale, const SDL_Color& tint)
{
    SDL_RenderCopyEx(renderer, texture, const SDL_Rect* srcrect,
                     const SDL_Rect* dstrect, rotation, const SDL_Point* center,
                     const SDL_RendererFlip flip);
}

// TODO: Remove this function.
void RenderWindow::render(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst)
{
    SDL_RenderCopy(renderer, texture, src, dst);
}

void RenderWindow::display() { SDL_RenderPresent(renderer); }

void RenderWindow::close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}