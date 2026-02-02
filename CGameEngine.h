#pragma once

#ifndef CGAMEENGINE_H
#define CGAMEENGINE_H
#include "CCounter.h"
#include "CGameSubsystem.h"
#include "CInputSubsystem.h"
#include "CRenderSubsystem.h"

class CGameEngine
{
public:
    static CGameEngine& GetInstance()
    {
        static CGameEngine instance;
        return instance;
    }
    
    bool Init()
    {
        printf("Game Engine Init\n");
        
        renderSubsystem = std::make_unique<CRenderSubsystem>();
        inputSubsystem = std::make_unique<CInputSubsystem>();
        gameSubsystem = std::make_unique<CGameSubsystem>();

        gameSubsystem->SetInputSubsystem(inputSubsystem.get());
        
        if (!renderSubsystem->Start())
        {
            printf("RenderSubsystem Start Error\n");
            return false;
        }

        if (!inputSubsystem->Start())
        {
            printf("InputSubsystem Start Error\n");
            return false;
        }

        if (!gameSubsystem->Start())
        {
            printf("GameSubsystem Start Error\n");
            return false;
        }
        
        counter.Start();

        printf("Game Engine Init Done\n");
        
        return true;
    }

    void Loop()
    {
        while (!inputSubsystem->ShouldQuit())
        {
            renderSubsystem->OnBeginFrame(gameSubsystem->GetRed(), gameSubsystem->GetGreen(), gameSubsystem->GetBlue());
            
            counter.Tick();
            float deltaSeconds = counter.GetDeltaSeconds();
        
            renderSubsystem->Update(deltaSeconds);
            inputSubsystem->Update(deltaSeconds);
            gameSubsystem->Update(deltaSeconds);
            
            renderSubsystem->Render(gameSubsystem->GetAverageFPS(), gameSubsystem->GetAverageDeltaTime());

            renderSubsystem->OnEndFrame();
        }
    }
    
    void Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        if (renderSubsystem)
        {
            SDL_GL_DestroyContext(renderSubsystem->GetGLContext());
            SDL_DestroyWindow(renderSubsystem->GetWindow());
        }

        SDL_Quit();

        if (renderSubsystem)
        {
            renderSubsystem->Shutdown();
            renderSubsystem.reset();
        }
        
        if (inputSubsystem)
        {
            inputSubsystem->Shutdown();
            inputSubsystem.reset();
        }

        if (gameSubsystem)
        {
            gameSubsystem->Shutdown();
            gameSubsystem.reset();
        }
    }

    CRenderSubsystem* GetRenderer() { return renderSubsystem.get(); }

    CInputSubsystem* GetInput() { return inputSubsystem.get(); }

    CGameSubsystem* GetGame() { return gameSubsystem.get(); }

private:
    CGameEngine() = default;
    
    ~CGameEngine() = default;
    
    std::unique_ptr<CRenderSubsystem> renderSubsystem;
    std::unique_ptr<CInputSubsystem> inputSubsystem;
    std::unique_ptr<CGameSubsystem> gameSubsystem;
    CCounter counter;
};

#endif