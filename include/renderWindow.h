#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include "SDL.h"

#include <string_view>

class RenderWindow
{
public:
    /**
     * @brief Initializes the window.
     * @param width Window width.
     * @param height Window height.
     * @param title Window title.
     */
    RenderWindow(int width, int height, std::string_view title);

    /**
     * @brief Sets background color.
     * @param color
     */
    void clear(const SDL_Color& color);

    /**
     * @brief Loads image as texture.
     * @param path Image file path.
     * @return Texture
     */
    SDL_Texture* loadTexture(std::string_view path);

    /**
     * @brief Renders a texture.
     * @param texture
     * @param posX
     * @param posY
     * @param tint Default = null
     */
    void render(SDL_Texture* texture, float posX, float posY,
                const SDL_Color* tint = nullptr);

    /**
     * @brief Renders a texture with position.
     * @param texture
     * @param position
     * @param tint Default = null
     */
    void render(SDL_Texture* texture, const SDL_FPoint& position,
                const SDL_Color* tint = nullptr);

    /**
     * @brief Renders a texture with extended parameters.
     * @param texture The source texture.
     * @param position The texture position.
     * @param angle An angle in degrees that indicates the rotation that will be
     * applied to dest, rotating it in a clockwise direction.
     * @param center A pointer to a point indicating the point around which
     * dest will be rotated (if NULL, rotation will be done around
     * dest.w / 2, dest.h / 2).
     * @param scale Texture scale. Default = { 1, 1 }
     * @param tint Additional color value. By default, it's null.
     */
    void render(SDL_Texture* texture, const SDL_FPoint& position, double angle,
                const SDL_FPoint* center,
                const SDL_FPoint& scale = { 1.0, 1.0 },
                const SDL_Color* tint = nullptr);

    /**
     * @brief Renders a part of a texture defined by a rectangle.
     * @param texture
     * @param source
     * @param position
     * @param tint Default = null
     */
    void render(SDL_Texture* texture, const SDL_Rect* source,
                const SDL_FPoint& position, const SDL_Color* tint = nullptr);

    /**
     * @brief Renders a part of a texture defined by a rectangle with 'pro'
     * parameters.
     * @param texture The source texture.
     * @param source the source SDL_Rect structure or NULL for the entire
     * texture.
     * @param dest The destination SDL_Rect structure or NULL for the entire
     * rendering target.
     * @param angle An angle in degrees that indicates the rotation that will be
     * applied to dest, rotating it in a clockwise direction.
     * @param center A pointer to a point indicating the point around which
     * dest will be rotated (if NULL, rotation will be done around
     * dest.w / 2, dest.h / 2).
     * @param tint Additional color value. By default, it's null.
     */
    void render(SDL_Texture* texture, const SDL_Rect* source,
                const SDL_FRect* dest, double angle, const SDL_FPoint* center,
                const SDL_Color* tint = nullptr);

    /**
     * @brief Displays screen.
     */
    void display();

    /**
     * @brief Shuts down SDL.
     */
    void close();

private:
    SDL_Window* window { nullptr };
    SDL_Renderer* renderer { nullptr };
};

#endif
