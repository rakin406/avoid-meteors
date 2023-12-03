#ifndef TOOLS_H
#define TOOLS_H

#include "SDL.h"

namespace tools
{
    /**
     * @brief Gets a random value between min and max (both included).
     * @param min Minimum integer.
     * @param max Maximum integer.
     * @return Random integer.
     */
    int getRandomValue(int min, int max);

    /**
     * @brief Gets size of texture.
     * @param texture
     * @return Size
     */
    SDL_Point getSize(SDL_Texture* texture);
} // namespace tools

#endif