#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include "components.h"

#include "SDL.h"

#include <string_view>

class RenderWindow
{
public:
    /**
     * @brief Initialize the window.
     * @param width Window width.
     * @param height Window height.
     * @param title Window title.
     */
    RenderWindow(int width, int height, std::string_view title);

    /**
     * @brief Set background color.
     * @param color
     */
    void clear(const SDL_Color& color);

    /**
     * @brief Load texture from file.
     * @param fileName Image file name.
     * @return texture Image texture.
     */
    SDL_Texture* loadTexture(std::string_view fileName);

    /**
     * @brief Render a texture.
     * @param texture
     * @param posX
     * @param posY
     * @param tint
     */
    void render(SDL_Texture* texture, int posX, int posY,
                const SDL_Color& tint);

    /**
     * @brief Render a texture with position.
     * @param texture
     * @param position
     * @param tint
     */
    void renderV(SDL_Texture* texture, const Position& position,
                 const SDL_Color& tint);

    /**
     * @brief Render a texture with extended parameters.
     * @param texture
     * @param position
     * @param rotation
     * @param scale
     * @param tint
     */
    void renderEx(SDL_Texture* texture, const Position& position,
                  float rotation, float scale, const SDL_Color& tint);

    /**
     * @brief Render a part of a texture defined by a rectangle.
     * @param texture
     * @param source
     * @param position
     * @param tint
     */
    void renderRec(SDL_Texture* texture, SDL_Rect* source,
                   const Position& position, const SDL_Color& tint);

    /**
     * @brief Render a part of a texture defined by a rectangle with 'pro'
     * parameters.
     * @param texture
     * @param source
     * @param dest
     * @param origin
     * @param rotation
     * @param tint
     */
    void renderPro(SDL_Texture* texture, SDL_Rect* source, SDL_Rect* dest,
                   const Position& origin, float rotation,
                   const SDL_Color& tint);

    /**
     * @brief Display screen.
     */
    void display();

    /**
     * @brief Close window.
     */
    void close();

private:
    SDL_Window* window { nullptr };
    SDL_Renderer* renderer { nullptr };
};

#endif
