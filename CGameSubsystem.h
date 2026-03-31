#pragma once

#ifndef CGAMESUBSYSTEM_H
#define CGAMESUBSYSTEM_H

#include "ISubsystem.h"
#include <iostream>

#include "CInputSubsystem.h"
#include "CRenderSubsystem.h"
#include "CEntity.h"
#include "PrimeWorker.h"

class CGameSubsystem : public ISubsystem
{
public:
    CGameSubsystem(): frameIndex(0), frameCount(0), timeSinceLastUpdate(0.0f), averageFPS(0.0f), averageDeltaTime(0.0f), inputSubsystem(nullptr), red(0), green(0), blue(0), timeSinceLastPoll(0.0f)
    {
        for (int i = 0; i < 100; ++i)
            frameTimes[i] = 0.0f;
    }

    void SetInputSubsystem(CInputSubsystem* iInputSubsystem)
    {
        inputSubsystem = iInputSubsystem;
    }
    
    ~CGameSubsystem() override
    {
        Shutdown();
    }
    
    bool Start() override
    {
        printf("Game Subsystem Started\n");
        return true;
    }

    void Shutdown() override
    {
        StopWorker(Worker);
    }

    void Update(float deltaSeconds) override
    {
        frameTimes[frameIndex] = deltaSeconds;
        frameIndex = (frameIndex + 1) % 100;

        if (frameCount < 100)
            frameCount++;

        timeSinceLastUpdate += deltaSeconds;

        if (timeSinceLastUpdate >= 0.5f)
        {
            float sum = 0.0f;

            for (int i = 0; i < frameCount; ++i)
                sum += frameTimes[i];

            averageDeltaTime = sum / static_cast<float>(frameCount);

            if (averageDeltaTime > 0.0f)
                averageFPS = 1.0f / averageDeltaTime;

            timeSinceLastUpdate = 0.0f;
        }

        if (inputSubsystem != nullptr)
            if (inputSubsystem->GetSpaceState() == EButtonState::JUST_PRESSED)
            {
                if (red == 128)
                {
                    red = 0;
                    green = 128;
                }
                else if (green == 128)
                {
                    green = 0;
                    blue = 128;
                }
                else
                {
                    blue = 0;
                    red = 128;
                }
            }

        //averageFPS = ImGui::GetIO().Framerate;
        //averageDeltaTime = 1000.0f / averageFPS;

        //Threads
        if (Worker.running.load())
        {
            timeSinceLastPoll += deltaSeconds;

            if (timeSinceLastPoll >= 2.0f)
            {
                timeSinceLastPoll = 0.0f;

                std::vector<int> newPrimes;

                PollWorker(Worker, newPrimes);
                
                for (int prime : newPrimes)
                    collectedPrimes.push_back(prime);
            }
        }
    }

    float GetAverageFPS() { return averageFPS; }

    float GetAverageDeltaTime() { return averageDeltaTime; }
    
    int GetRed() const { return red; }
    
    int GetGreen() const { return green; }
    
    int GetBlue() const { return blue; }

    std::vector<CEntity*>& GetEntities() { return Entities; }

    PrimeWorker& GetPrimeWorker() { return Worker; }
    std::vector<int>& GetCollectedPrimes() { return collectedPrimes; }
    
private:
    CInputSubsystem* inputSubsystem;
    
    float frameTimes[100];
    size_t frameIndex;
    size_t frameCount;
    float timeSinceLastUpdate;
    float averageFPS;
    float averageDeltaTime;
    
    int red;
    int green;
    int blue;

    std::vector<CEntity*>  Entities;

    PrimeWorker Worker;
    std::vector<int> collectedPrimes;
    float timeSinceLastPoll;
};

#endif
