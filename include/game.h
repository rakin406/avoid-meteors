#ifndef GAME_H
#define GAME_H

#include "renderWindow.h"

#include "SDL.h"
#include <flecs.h>

class Game
{
public:
    Game();
    void run();

private:
    bool running {};
    RenderWindow window;
    SDL_Texture* background {};
    SDL_Event event {};
    flecs::world world {};

    /**
     * @brief Initialize everything.
     */
    void init();

    /**
     * @brief Update and draw game.
     */
    void update();

    /**
     * @brief Stop all subsystems.
     */
    void stop();
};

#endif