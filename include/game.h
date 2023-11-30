#ifndef GAME_H
#define GAME_H

#include "renderWindow.h"

#include "SDL.h"

class Game
{
public:
    void run();

private:
    bool running { true };
    RenderWindow window;
    SDL_Texture* background { nullptr };
    SDL_Event event {};

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