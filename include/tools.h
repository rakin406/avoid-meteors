#ifndef TOOLS_H
#define TOOLS_H

#include "SDL.h"

namespace tools
{
    /**
     * @brief Get a random value between min and max (both included).
     * @param min 
     * @param max 
     * @return int
    */
    inline int getRandomValue(int min, int max);

    /**
     * @brief Get size of texture.
     * @param texture
     * @return size
     */
    SDL_Point getSize(SDL_Texture* texture);
} // namespace tools

#endif