#include "tools.h"

#include <random>

int tools::getRandomValue(int min, int max)
{
    static std::random_device rd {};
    static std::mt19937 mt { rd() };
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    return dist(mt);
}

SDL_FPoint tools::getSize(SDL_Texture* texture)
{
    SDL_FPoint size {};

    // Can't directly get size since it's float. Use integer pointers.
    int* width {};
    int* height {};
    SDL_QueryTexture(texture, nullptr, nullptr, width, height);

    size.x = *width;
    size.y = *height;

    return size;
}
