#pragma once
#include "CState.h"
class CTraceState :
    public CState
{
private:
    void SearchMovePath();
    vector<POINT> m_vPath;

public:
    virtual void update();
    void Trace(Vec2 vEnd);
    virtual void Enter();
    virtual void Exit();

public:
    CTraceState();
    ~CTraceState();
};

