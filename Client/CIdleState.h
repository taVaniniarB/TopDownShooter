#pragma once
#include "CState.h"
class CIdleState :
    public CState
{
private:
    float m_fCurDetectionDelay;
    float m_fDetectionDelay;

public:
    virtual void update();
    bool CheckWallInRay(Vec2& vEnd, Vec2& vStart);
    virtual void Enter();
    virtual void Exit();

public:
    CIdleState();
    ~CIdleState();
};

