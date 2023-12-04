#include "clock.h"

#include <chrono>

Clock::Clock()
    : lastTick { std::chrono::high_resolution_clock::now() }, deltaTime { 0 }
{
}

void Clock::tick()
{
    using namespace std::chrono;

    auto now { high_resolution_clock::now() };
    duration<float> elapsed { lastTick - now };
    deltaTime = elapsed.count();
    lastTick = now;
}

float Clock::getDeltaTime() const { return deltaTime; }
