#include "tools.h"

#include "SDL.h"

SDL_FPoint tools::getSize(SDL_Texture* texture)
{
    SDL_FPoint size {};

    // Can't directly get size since it's float. Use integers.
    int width {};
    int height {};
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    size.x = static_cast<float>(width);
    size.y = static_cast<float>(height);

    return size;
}
