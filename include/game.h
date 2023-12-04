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
