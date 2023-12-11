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

        static constexpr std::string_view BACKGROUND {
            PROJECT_ROOT "res/gfx/background.jpg"
        };

        static constexpr std::string_view DEFAULT_FONT {
            PROJECT_ROOT "res/font/Silkscreen/Silkscreen-Regular.ttf"
        };

        static constexpr int DEFAULT_FONT_SIZE { 20 };

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
