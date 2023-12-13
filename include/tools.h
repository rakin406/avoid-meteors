/**
 * @file
 * @brief Provides extra utility functions.
 */

#ifndef TOOLS_H
#define TOOLS_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <random>

namespace tools {
/**
 * @brief Gets size of texture.
 * @param texture
 * @return Size
 */
glm::vec2 getSize(SDL_Texture* texture);

/**
 * @brief Gets a random value between min and max (both included).
 * @tparam T
 * @param min
 * @param max
 * @return Random value.
 */
template <class T>
T getRandomValue(T min, T max) {
    static std::random_device rd{};
    static std::mt19937 mt{rd()};
    std::uniform_real_distribution<> dist(min, max);
    return static_cast<T>(dist(mt));
}

}  // namespace tools

#endif
