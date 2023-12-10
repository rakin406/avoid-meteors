/**
 * @file
 * @brief Provides score system module.
 */

#ifndef SCORE_SYSTEM_H
#define SCORE_SYSTEM_H

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
