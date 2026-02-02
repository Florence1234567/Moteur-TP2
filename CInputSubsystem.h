#pragma once

#ifndef C_INPUTSUBSYSTEM_H
#define C_INPUTSUBSYSTEM_H

#include "ISubsystem.h"
#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>

enum class EButtonState { UP, DOWN, JUST_PRESSED };

class CInputSubsystem : public ISubsystem
{
public:
    CInputSubsystem(): shouldQuit(false) {}

    ~CInputSubsystem()
    {
        Shutdown();
    }
    
    bool Start() override
    {
        printf("Input Subsystem Started\n");
        return true;
    }

    void Shutdown() override
    {
        
    }

    void Update(float deltaSeconds) override
    {
        (void)deltaSeconds;
        
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            ProcessEvent(&event);
        }

        if (spaceState == EButtonState::DOWN && prevSpaceState == EButtonState::UP)
            spaceState = EButtonState::JUST_PRESSED;
        else if (spaceState == EButtonState::JUST_PRESSED)
            spaceState = EButtonState::DOWN;

        prevSpaceState = spaceState;
    }

    bool ShouldQuit() const { return shouldQuit; }

    void ProcessEvent(const SDL_Event* event)
    {
        switch (event->type)
        {
        case SDL_EVENT_QUIT:
                shouldQuit = true;  /* end the program, reporting success to the OS. */
            break;
        case SDL_EVENT_KEY_DOWN:
            if (event->key.key == SDLK_SPACE)
                spaceState = EButtonState::DOWN;
            break;
        case SDL_EVENT_KEY_UP:
            if (event->key.key == SDLK_SPACE)
                spaceState = EButtonState::UP;
            break;
        default:
            break;
        }
        printf("Event handeled\n");
    }

    EButtonState GetSpaceState() const { return spaceState; }
private:
    bool shouldQuit;
    EButtonState spaceState = EButtonState::UP;
    EButtonState prevSpaceState = EButtonState::UP;
};

#endif
