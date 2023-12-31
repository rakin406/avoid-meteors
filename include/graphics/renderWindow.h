/**
 * @file
 * @brief Provides RenderWindow class to initialize window and do rendering.
 */

#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include "colors.h"

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <glm/glm.hpp>

#include <string_view>

/**
 * @brief Window that serves as a target for 2D drawing.
 */
class RenderWindow {
  public:
    /**
     * @brief Initializes the window.
     * @param width Window width.
     * @param height Window height.
     * @param title Window title.
     */
    RenderWindow(int width, int height, std::string_view title);

    ~RenderWindow();

    /**
     * @brief Gets the size of the rendering region of the window.
     * @return Size.
     */
    glm::ivec2 getSize() const;

    /**
     * @brief Clears the entire target with a single color.
     * @param color Default is black.
     */
    void clear(const SDL_Color& color = BLACK);

    /**
     * @brief Loads image as texture.
     * @param path Image file path.
     * @return Texture
     */
    SDL_Texture* loadTexture(std::string_view path);

    /**
     * @brief Loads text as texture.
     * @param textureText
     * @param fontSize
     * @param textColor
     * @return Texture
     */
    SDL_Texture* loadTexture(std::string_view textureText,
                             int fontSize,
                             const SDL_Color& textColor);

    /**
     * @brief Loads font from file.
     * @param path Path to font file.
     * @param size Point size to use for the newly-opened font.
     * @return True if successful.
     */
    bool loadFont(std::string_view path, int size);

    /**
     * @brief Renders a texture.
     * @param texture
     * @param posX
     * @param posY
     * @param tint Default = null
     */
    void render(SDL_Texture* texture,
                float posX,
                float posY,
                const SDL_Color* tint = nullptr);

    /**
     * @brief Renders a texture with position.
     * @param texture
     * @param position
     * @param tint Default = null
     */
    void render(SDL_Texture* texture,
                const glm::vec2& position,
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
    void render(SDL_Texture* texture,
                const glm::vec2& position,
                double angle,
                const glm::vec2* center,
                const glm::vec2& scale = {1.0f, 1.0f},
                const SDL_Color* tint = nullptr);

    /**
     * @brief Renders a part of a texture defined by a rectangle.
     * @param texture
     * @param source
     * @param position
     * @param tint Default = null
     */
    void render(SDL_Texture* texture,
                const SDL_Rect* source,
                const glm::vec2& position,
                const SDL_Color* tint = nullptr);

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
     * @param flip A SDL_RendererFlip value stating which flipping actions
     * should be performed on the texture.
     * @param tint Additional color value. By default, it's null.
     */
    void render(SDL_Texture* texture,
                const SDL_Rect* source,
                const SDL_FRect* dest,
                double angle,
                const glm::vec2* center,
                const SDL_RendererFlip& flip = SDL_FLIP_NONE,
                const SDL_Color* tint = nullptr);

    /**
     * @brief Displays on screen what has been rendered to the window
     * so far.
     */
    void display();

    /**
     * @brief Closes the window and destroys all the attached resources.
     */
    void close();

  private:
    SDL_Window* window{nullptr};
    SDL_Renderer* renderer{nullptr};
    TTF_Font* font{nullptr};
};

#endif
