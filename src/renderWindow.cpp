#include "renderWindow.h"

#include "SDL.h"
#include "SDL_image.h"

#include <iostream>
#include <string_view>

RenderWindow::RenderWindow(std::string_view title, int width, int height)
{
    window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width, height,
                              SDL_WINDOW_SHOWN);

    if (window == nullptr)
    {
        std::cout << "Window failed to init. Error: " << SDL_GetError()
                  << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

// TODO: Put this function somewhere else.
/**
 * @brief Return true if user requests quit. For use in main loop.
 *
 * @return boolean.
 */
// bool isQuitRequested()
//{
//     SDL_Event event {};
//
//     // Check for events
//     while (SDL_PollEvent(&event) != 0)
//     {
//         if (event.type == SDL_QUIT)
//             return true;
//
//         if (event.type == SDL_KEYDOWN)
//         {
//             if (event.key.keysym.sym == SDLK_ESCAPE)
//                 return true;
//         }
//     }
//
//     return false;
// }

void RenderWindow::clear(Uint8 red, Uint8 green, Uint8 blue)
{
    SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

SDL_Texture* RenderWindow::loadTexture(std::string_view filePath)
{
    SDL_Texture* texture { IMG_LoadTexture(renderer, filePath.data()) };

    if (texture == nullptr)
    {
        std::cout << "Failed to load texture. Error: " << SDL_GetError()
                  << std::endl;
    }

    return texture;
}

void RenderWindow::display() { SDL_RenderPresent(renderer); }

void RenderWindow::destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}