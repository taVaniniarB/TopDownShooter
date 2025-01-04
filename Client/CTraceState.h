#pragma once
#include "CState.h"
class CTraceState :
    public CState
{
private:
    void SearchMovePath_AStar(Vec2 vEnd, Vec2 vStart);
    bool isWallInPath(Vec2 vEnd, Vec2 vStart);
    vector<Vec2> m_vPath;

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

