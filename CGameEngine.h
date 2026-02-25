#pragma once

#ifndef CGAMEENGINE_H
#define CGAMEENGINE_H
#include "CCounter.h"
#include "CGameSubsystem.h"
#include "CInputSubsystem.h"
#include "CRenderSubsystem.h"
#include "CPageAllocator.h"
#include "GAnimal.h"
#include "GCat.h"
#include "GDog.h"

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

        GAnimal::RegisterProperties({ "name", EPropertyType::String, offsetof(GAnimal, name) });
        
        cat1 = NewObject<GCat>("Oreo");
        cat2 = NewObject<GCat>("Daisy");
        dog1 = NewObject<GDog>("Violette");
        dog2 = NewObject<GDog>("Violet");
        
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
            
            renderSubsystem->Render(
                gameSubsystem->GetAverageFPS(),
                gameSubsystem->GetAverageDeltaTime(),
                gameSubsystem->GetEntities(),
                pageAllocator.GetAvailable(),
                pageAllocator.GetUsed(),
                [this]() { return NewObject<CEntity>(); },
                [this]() { return NewObject<CComponent>(); },
                [this](CEntity* entity) { FreeObject(entity); },
                [this](CComponent* component) { FreeObject(component); });

            renderSubsystem->OnEndFrame();
        }
    }
    
    void Shutdown()
    {
        FreeObject(cat1);
        FreeObject(cat2);
        FreeObject(dog1);
        FreeObject(dog2);
        
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

    template<typename T, typename... Args>
    T* NewObject(Args&&... args) { return pageAllocator.NewObject<T>(std::forward<Args>(args)...); }

    template<typename T>
    void FreeObject(T* object) { pageAllocator.FreeObject(object); }

private:
    CGameEngine() = default;
    
    ~CGameEngine() = default;
    
    std::unique_ptr<CRenderSubsystem> renderSubsystem;
    std::unique_ptr<CInputSubsystem> inputSubsystem;
    std::unique_ptr<CGameSubsystem> gameSubsystem;
    CCounter counter;

    CPageAllocator<128, 1024 * 1024> pageAllocator;

    GCat* cat1 = nullptr;
    GCat* cat2 = nullptr;
    GDog* dog1 = nullptr;
    GDog* dog2 = nullptr;
};

#endif