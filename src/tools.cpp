#include "tools.h"

SDL_Point tools::getSize(SDL_Texture* texture)
{
    SDL_Point size {};
    SDL_QueryTexture(texture, nullptr, nullptr, &size.x, &size.y);
    return size;
}