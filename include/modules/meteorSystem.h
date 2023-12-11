/**
 * @file
 * @brief Provides meteor system module.
 */

#ifndef METEOR_SYSTEM_H
#define METEOR_SYSTEM_H

#include <flecs.h>

#include <string_view>

namespace modules
{
    /**
     * @brief MeteorSystem module.
     */
    class MeteorSystem
    {
    public:
        // -- Constants

        static constexpr std::string_view METEOR_SPRITE {
            PROJECT_ROOT "res/gfx/meteor.png"
        };
        static constexpr float MIN_SCALE { 1.0f };
        static constexpr float MAX_SCALE { 5.0f };
        static constexpr float SPEED { 300.0f };

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
