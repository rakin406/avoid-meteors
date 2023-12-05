/**
 * @file
 * @brief Provides player module.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "renderWindow.h"

#include "SDL.h"
#include <flecs.h>

namespace modules
{
    /**
     * @brief Player module.
     */
    class Player
    {
    public:
        flecs::entity player {};

        /**
         * @brief Initializes player and it's systems.
         * @param world
         * @param window
         */
        Player(flecs::world& world, RenderWindow& window);
    };
} // namespace modules

#endif
