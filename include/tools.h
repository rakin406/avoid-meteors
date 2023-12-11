/**
 * @file
 * @brief Provides extra utility functions.
 */

#ifndef TOOLS_H
#define TOOLS_H

#include "SDL.h"

#include <random>

namespace tools
{
    /**
     * @brief Gets size of texture.
     * @param texture
     * @return Size
     */
    SDL_FPoint getSize(SDL_Texture* texture);

    /**
     * @brief Gets a random value between min and max (both included).
     * @tparam T
     * @param min
     * @param max
     * @return Random value.
     */
    template <class T>
    T getRandomValue(T min, T max)
    {
        static std::random_device rd {};
        static std::mt19937 mt { rd() };
        std::uniform_real_distribution<T> dist(min, max);
        return dist(mt);
    }

} // namespace tools

#endif
