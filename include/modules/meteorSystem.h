/**
 * @file
 * @brief Provides meteor system module.
 */

#ifndef METEOR_SYSTEM_H
#define METEOR_SYSTEM_H

#include <flecs.h>

namespace modules
{
    /**
     * @brief MeteorSystem module.
     */
    class MeteorSystem
    {
    public:
        // -- Constants

        // -- Components

        /**
         * @brief Module import function.
         * @param world
         */
        MeteorSystem(flecs::world& world);

    private:
        /**
         * @brief Initializes meteor entities and components.
         * @param world
         */
        static void meteorsInit(flecs::world& world);
    };
} // namespace modules

#endif
