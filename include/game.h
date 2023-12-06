/**
 * @file
 * @brief Contains Game class which provides the entry point to the game.
 */

#ifndef GAME_H
#define GAME_H

#include <flecs.h>

class Game
{
public:
    void run();

private:
    flecs::world world {};

    /**
     * @brief Initializes everything.
     */
    void init();

    /**
     * @brief Stops all subsystems.
     */
    void stop();
};

#endif
