#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

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
     *
     * @param color
     */
    void clear(const SDL_Color& color);

    /**
     * @brief Load texture from file.
     *
     * @param fileName Image file name.
     * @return texture Image texture.
     */
    SDL_Texture* loadTexture(std::string_view fileName);

    /**
     * @brief Render texture on screen.
     * @param texture
     * @param x
     * @param y
     */
    void render(SDL_Texture* texture, int x, int y);

    /**
     * @brief Render texture on screen.
     * @param texture
     * @param src Source
     * @param dst Destination
     */
    void render(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst);

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
