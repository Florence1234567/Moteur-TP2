#pragma once

#ifndef ISUBSYSTEM_H
#define ISUBSYSTEM_H

class ISubsystem
{
public:
    virtual ~ISubsystem() = default;

    virtual bool Start() = 0;

    virtual void Shutdown() = 0;

    virtual void Update(float deltaSeconds) = 0;
};

#endif
