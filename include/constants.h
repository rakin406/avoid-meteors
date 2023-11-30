#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "states.h"

#include <array>
#include <string_view>

namespace constants
{
    inline constexpr int WINDOW_WIDTH { 1280 };
    inline constexpr int WINDOW_HEIGHT { 720 };

    inline constexpr std::string_view BG_IMG_PATH { PROJECT_ROOT
                                                    "res/gfx/background.jpg" };

    // Based off of background texture ground
    inline constexpr int GROUND_POS_Y { WINDOW_HEIGHT - 10 };

    inline constexpr std::array<Direction, 2> ALL_DIRECTIONS {
        Direction::Left, Direction::Right
    };

    inline constexpr int PLAYER_SPEED { 1 };
} // namespace constants

#endif