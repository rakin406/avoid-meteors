/**
 * @file
 * @brief Provides render system module.
 */

#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <flecs.h>

#include <string_view>

namespace modules
{
    /**
     * @brief RenderSystem module.
     */
    class RenderSystem
    {
    public:
        // -- Constants

        static constexpr int WINDOW_WIDTH { 1280 };
        static constexpr int WINDOW_HEIGHT { 720 };
        static constexpr std::string_view WINDOW_TITLE { "Avoid Meteors" };

        static constexpr std::string_view BACKGROUND {
            PROJECT_ROOT "res/gfx/background.jpg"
        };

        // -- Components

        struct Background
        {
        };

        /**
         * @brief Module import function.
         * @param world
         */
        RenderSystem(flecs::world& world);

    private:
        /**
         * @brief Initializes window.
         * @param world
         */
        static void windowInit(flecs::world& world);
    };
} // namespace modules

#endif
