#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

class Clock
{
public:
    Clock();

    /**
     * @brief Updates clock one tick.
     */
    void tick();

    /**
     * @brief Gets delta time.
     * @return delta time.
     */
    float getDeltaTime() const;

private:
    std::chrono::high_resolution_clock::time_point lastTick {};
    float deltaTime {};
};

#endif
