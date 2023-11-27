#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string_view>

namespace constants
{
    inline constexpr int WINDOW_WIDTH { 1280 };
    inline constexpr int WINDOW_HEIGHT { 720 };
    inline constexpr std::string_view BG_IMG_PATH { PROJECT_ROOT
                                                    "res/gfx/background.jpg" };
} // namespace constants

#endif