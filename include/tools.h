#ifndef TOOLS_H
#define TOOLS_H

#include "SDL.h"

namespace tools
{
    /**
     * @brief Get size of texture.
     * @param texture
     * @return size
     */
    SDL_Point getSize(SDL_Texture* texture);
} // namespace tools

#endif