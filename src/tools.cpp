#include "tools.h"

#include <random>

int tools::getRandomValue(int min, int max)
{
    static std::random_device rd {};
    static std::mt19937 mt { rd() };
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    return dist(mt);
}

SDL_Point tools::getSize(SDL_Texture* texture)
{
    SDL_Point size {};
    SDL_QueryTexture(texture, nullptr, nullptr, &size.x, &size.y);
    return size;
}