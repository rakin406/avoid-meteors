/**
 * @file
 * @brief Provides score system module.
 */

#ifndef SCORE_SYSTEM_H
#define SCORE_SYSTEM_H

#include "../colors.h"

#include <flecs.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

namespace modules
{
    /**
     * @brief ScoreSystem module.
     */
    class ScoreSystem
    {
    public:
        // -- Constants

        static constexpr int FONT_SIZE { 30 };
        static constexpr SDL_Color TEXT_COLOR { BLACK };
        static constexpr glm::vec2 TEXT_POSITION { 10.0f, 10.0f };

        // -- Components

        using Score = int;

        /**
         * @brief Module import function.
         * @param world
         */
        ScoreSystem(flecs::world& world);

    private:
        /**
         * @brief Sets score and components.
         * @param world
         */
        static void scoreInit(flecs::world& world);
    };
} // namespace modules

#endif
