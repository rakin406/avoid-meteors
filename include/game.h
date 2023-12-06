/**
 * @file
 * @brief Contains Game class which provides the entry point to the game.
 */

#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include <flecs.h>

class Game
{
public:
    Game();
    void run();

private:
    bool running {};
    SDL_Event event {};
    flecs::world world {};

    /**
     * @brief Initializes everything.
     */
    void init();

    /**
     * @brief Updates and draws game.
     */
    void update();

    /**
     * @brief Stops all subsystems.
     */
    void stop();
};

#endif
