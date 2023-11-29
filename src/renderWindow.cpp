#include "renderWindow.h"
#include "components.h"

#include "SDL.h"
#include "SDL_image.h"

#include <cmath>
#include <iostream>
#include <string_view>

namespace
{
    /**
     * @brief Get size of texture.
     * @param texture
     * @return size
     */
    SDL_Point getSize(SDL_Texture* texture)
    {
        SDL_Point size {};
        SDL_QueryTexture(texture, nullptr, nullptr, &size.x, &size.y);
        return size;
    }

    /**
     * @brief Convert degree to radian.
     * @param d
     * @return radian
     */
    double deg2Rad(double d) { return (d / 180.0) * static_cast<double>(M_PI); }
} // namespace

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
    renderEx(texture, { posX, posY }, 0, 1, tint);
}

void RenderWindow::renderV(SDL_Texture* texture, const Position& position,
                           const SDL_Color& tint)
{
    renderEx(texture, position, 0, 1, tint);
}

void RenderWindow::renderEx(SDL_Texture* texture, const Position& position,
                            float rotation, float scale, const SDL_Color& tint)
{
    SDL_Point textureSize { getSize(texture) };
    SDL_Rect source { 0, 0, textureSize.x, textureSize.y };
    SDL_Rect dest { position.x, position.y, textureSize.x * scale,
                    textureSize.y * scale };
    Position origin { 0, 0 };

    renderPro(texture, &source, &dest, origin, rotation, tint);
}

void RenderWindow::renderRec(SDL_Texture* texture, SDL_Rect* source,
                             const Position& position, const SDL_Color& tint)
{
    SDL_Rect dest { position.x, position.y, source->w, source->h };
    Position origin { 0, 0 };

    renderPro(texture, source, &dest, origin, 0.0f, tint);
}

// NOTE: origin is relative to destination rectangle size
void RenderWindow::renderPro(SDL_Texture* texture, SDL_Rect* source,
                             SDL_Rect* dest, const Position& origin,
                             float rotation, const SDL_Color& tint)
{
    // Check if texture is valid
    if (SDL_QueryTexture(texture, nullptr, nullptr, nullptr, nullptr))
    {
        SDL_Point size { getSize(texture) };
        int width { size.x };
        int height { size.y };

        bool flipX { false };

        if (source->w < 0)
        {
            flipX = true;
            source->w *= -1;
        }
        if (source->h < 0)
            source->y -= source->h;

        Position topLeft { 0 };
        Position topRight { 0 };
        Position bottomLeft { 0 };
        Position bottomRight { 0 };

        // Only calculate rotation if needed
        if (rotation == 0.0f)
        {
            int x { dest->x - origin.x };
            int y { dest->y - origin.y };
            topLeft = { x, y };
            topRight = { x + dest->w, y };
            bottomLeft = { x, y + dest->h };
            bottomRight = { x + dest->w, y + dest->h };
        }
        else
        {
            float sinRotation { std::sinf(deg2Rad(rotation)) };
            float cosRotation { std::cosf(deg2Rad(rotation)) };
            float x { dest->x };
            float y { dest->y };
            float dx { -origin.x };
            float dy { -origin.y };

            topLeft.x = x + dx * cosRotation - dy * sinRotation;
            topLeft.y = y + dx * sinRotation + dy * cosRotation;

            topRight.x = x + (dx + dest->w) * cosRotation - dy * sinRotation;
            topRight.y = y + (dx + dest->w) * sinRotation + dy * cosRotation;

            bottomLeft.x =
                x + dx * cosRotation - (dy + dest->h) * sinRotation;
            bottomLeft.y =
                y + dx * sinRotation + (dy + dest->h) * cosRotation;

            bottomRight.x = x + (dx + dest->w) * cosRotation -
                            (dy + dest->h) * sinRotation;
            bottomRight.y = y + (dx + dest->w) * sinRotation +
                            (dy + dest->h) * cosRotation;
        }

        SDL_SetTextureColorMod(texture, tint.r, tint.g, tint.b);
        // TODO: Add more code here.
    }
}

void RenderWindow::display() { SDL_RenderPresent(renderer); }

void RenderWindow::close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}