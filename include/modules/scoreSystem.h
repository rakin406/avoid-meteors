/**
 * @file
 * @brief Provides score system module.
 */

#ifndef SCORE_SYSTEM_H
#define SCORE_SYSTEM_H

#include "../colors.h"
#include "../components.h"

#include "SDL.h"
#include <flecs.h>
#include <SDL_ttf.h>

#include <string_view>

namespace modules
{
    /**
     * @brief ScoreSystem module.
     */
    class ScoreSystem
    {
    public:
        // -- Constants

        static constexpr int FONT_SIZE { 15 };
        static constexpr SDL_Color TEXT_COLOR { BLACK };
        static constexpr SDL_FPoint TEXT_POSITION { 10.0f, 10.0f };

        // -- Components

        using Score = int;

        /**
         * @brief Contains information about the text to be displayed on the
         * screen.
         */
        struct Text
        {
            std::string_view content {}; // The actual text to be displayed
            int size {};                 // The size of the font
            SDL_Color color {};          // The color of the text
            SDL_FPoint position {};      // The position of the text
        };

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
