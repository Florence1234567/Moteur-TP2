#pragma once

#ifndef CRENDER_SUBSYSTEM_H
#define CRENDER_SUBSYSTEM_H

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>

#include "ISubsystem.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>

class CRenderSubsystem : public ISubsystem
{
public:
    CRenderSubsystem(): window(nullptr), renderer(nullptr) {}

    ~CRenderSubsystem() override
    {
        Shutdown();
    }
    
    bool Start() override
    {
        SDL_Init(SDL_INIT_VIDEO);
        
        float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
        SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;

        window = SDL_CreateWindow("Hello World", (int)(1280 * main_scale), (int)(800 * main_scale), window_flags);

        if (!window)
            return false;

        gl_context = SDL_GL_CreateContext(window);
        SDL_GL_SetSwapInterval(1);
        
        ImGui::CreateContext();
        ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
        ImGui_ImplOpenGL3_Init("#version 130");

        printf("Render Subsystem Started\n");
        return true;
    }

    void Shutdown() override
    {
        window = nullptr;
        renderer = nullptr;
    }

    void Update(float deltaSeconds) override
    {
        (void)deltaSeconds;
    }

    void Render(float averageFPS, float averageDeltaTime)
    {
        ImGui::Begin("Hello World");
        ImGui::Text("FPS: %f\n", averageFPS);
        ImGui::Text("Avg Delta: %.4f s", averageDeltaTime);
        ImGui::End();
    }
    
    SDL_Window* GetWindow() { return window; }

    SDL_Renderer* GetRenderer() { return renderer; }

    SDL_GLContext GetGLContext() { return gl_context; }

    void OnBeginFrame(int red, int green, int blue)
    {
        glClearColor(red, green, blue, 255);
        glClear(GL_COLOR_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void OnEndFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        SDL_GL_SwapWindow(window);
    }
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_GLContext gl_context;
};

#endif
