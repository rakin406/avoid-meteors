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
        static constexpr float INITIAL_SPEED { 300.0f };
        static constexpr double ROTATION_SPEED { 2 };

        // -- Components

        using Angle = float;

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