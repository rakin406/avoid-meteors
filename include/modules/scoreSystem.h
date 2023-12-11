/**
 * @file
 * @brief Provides score system module.
 */

#ifndef SCORE_SYSTEM_H
#define SCORE_SYSTEM_H

#include "../colors.h"

#include "SDL.h"
#include <flecs.h>

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
        static constexpr SDL_FPoint TEXT_POSITION { 10.0f, 10.0f };

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
