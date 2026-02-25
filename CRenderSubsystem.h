#pragma once

#ifndef CRENDER_SUBSYSTEM_H
#define CRENDER_SUBSYSTEM_H

#include <functional>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>

#include "ISubsystem.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>

#include "CEntity.h"
#include <functional>

class CRenderSubsystem : public ISubsystem
{
public:
    CRenderSubsystem(): window(nullptr) {}

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
    }

    void Update(float deltaSeconds) override
    {
        (void)deltaSeconds;
    }

    void Render(
        float averageFPS,
        float averageDeltaTime,
        std::vector<CEntity*>& entities,
        int available,
        int used,
        std::function<CEntity*()> NewEntity,
        std::function<CComponent*()> NewComponent,
        std::function<void(CEntity*)> FreeEntity,
        std::function<void(CComponent*)> FreeComponent)
    {
        //ImGui::ShowDemoWindow();

        ImGui::Begin("Hello World");
        ImGui::Text("FPS: %f\n", averageFPS);
        ImGui::Text("Avg Delta: %.4f s", averageDeltaTime);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
        ImGui::Begin("Allocator");
        ImGui::Text("Available: %d", available);
        ImGui::Text("Used: %d", used);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(10, 100), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_Once);
        ImGui::Begin("ECS");

        //Création d'une entité.
        static char entityName[128] = "Entity";
        ImGui::InputText("##name", entityName, sizeof(entityName));
        ImGui::SameLine();

        if (ImGui::Button("Add"))
        {
            CEntity* entity = NewEntity();
            entity->Name = entityName;
            entities.push_back(entity);
        }

        ImGui::Separator();

        //Liste des entités.
        static int selected = -1;

        for (int i = 0; i < (int)entities.size(); i++)
        {
            ImGui::PushID(i);
            // .c_str convertit un std::string en un const char*.
            if (ImGui::Selectable(entities[i]->Name.c_str(), selected == i))
                selected = i;
            ImGui::PopID();
        }

        ImGui::Separator();

        //Détails de l'entité séléctionnée.
        if (selected >= 0 && selected < (int)entities.size())
        {
            CEntity* entity = entities[selected];

            //Liste des components.
            ImGui::Text("%s's Components", entity->Name.c_str());
            for (int i = 0; i < (int)entity->Components.size(); i++)
                ImGui::Text("   %s", entity->Components[i]->Name.c_str());

            ImGui::Separator();
            
            //Ajouter un component.
            if (ImGui::Button("Add Component"))
            {
                CComponent* component = NewComponent();
                entity->Components.push_back(component);
            }
            
            //Détruire l'entité.
            if (ImGui::Button("Remove Entity"))
            {
                for (CComponent* component : entity->Components)
                    FreeComponent(component);

                FreeEntity(entity);
                entities.erase(entities.begin() + selected);
                selected = -1;
            }
        }
        
        ImGui::End();
    }
    
    SDL_Window* GetWindow() { return window; }
    
    SDL_GLContext GetGLContext() { return gl_context; }

    void OnBeginFrame(int red, int green, int blue)
    {
        glClearColor(red / 255.0f, green / 255.0f, blue / 255.0f, 1.0f);
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
    SDL_GLContext gl_context;
};

#endif
