#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include "SDL.h"

#include <string_view>

class RenderWindow
{
public:
    /**
     * @brief Initialize the window.
     * @param title Window title.
     * @param width Window width.
     * @param height Window height.
    */
    RenderWindow(std::string_view title, int width, int height);

    /**
     * @brief Clear screen with background color. Default is white.
     *
     * @param red Default = 255
     * @param green Default = 255
     * @param blue Default = 255
     */
    void clear(Uint8 red = 255, Uint8 green = 255, Uint8 blue = 255);

    /**
     * @brief Load texture from image.
     *
     * @param filePath Image
     * @return texture
     */
    SDL_Texture* loadTexture(std::string_view filePath);

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
     * @brief Kill and exit window.
     */
    void destroy();

private:
    SDL_Window* window { nullptr };
    SDL_Renderer* renderer { nullptr };
};

#endif
