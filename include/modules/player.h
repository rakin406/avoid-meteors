/**
 * @file
 * @brief Provides player module.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "../tags.h"

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

        Player(flecs::world& world);
    };
} // namespace modules

#endif
