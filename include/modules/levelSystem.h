/**
 * @file
 * @brief Provides level system module.
 */

#ifndef LEVEL_SYSTEM_H
#define LEVEL_SYSTEM_H

#include <flecs.h>

namespace modules
{
    /**
     * @brief LevelSystem module.
     */
    class LevelSystem
    {
    public:
        // -- Constants

        // -- Components

        /**
         * @brief Module import function.
         * @param world
         */
        LevelSystem(flecs::world& world);

    private:
        /**
         * @brief Sets level and components.
         * @param world
         */
        static void levelInit(flecs::world& world);
    };
} // namespace modules

#endif
