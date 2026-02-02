#pragma once

#ifndef  CCOUNTER_H
#define CCOUNTER_H
#include <SDL3/SDL_timer.h>

class CCounter
{
public:
    CCounter() : previousTick(0), currentTick(0), deltaSeconds(0.0f) {}

    void Start()
    {
        previousTick = SDL_GetTicks();
        currentTick = previousTick;
        deltaSeconds = 0.0f;
    }

    void Tick()
    {
        previousTick = currentTick;
        currentTick = SDL_GetTicks();

        uint64_t deltaTicks = currentTick - previousTick;
        deltaSeconds = (float)deltaTicks / 1000.0f;
    }

    float GetDeltaSeconds() const
    {
        return deltaSeconds;
    }
    
private:
    uint64_t previousTick;
    uint64_t currentTick;
    float deltaSeconds;
};

#endif
