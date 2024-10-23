#pragma once
#include "CState.h"
class CTraceState :
    public CState
{
private:
    void SearchMovePath(Vec2 vEnd, Vec2 vStart);
    bool isWallInPath(Vec2 vEnd, Vec2 vStart);
    vector<POINT> m_vPath;

    //Vec2 m_vColliderSize;
    //Vec2 m_vPrevDir;

public:
    virtual void update();
    void Attack();
    void Trace(Vec2 vEnd);
    virtual void Enter();
    virtual void Exit();

public:
    CTraceState();
    ~CTraceState();
};

