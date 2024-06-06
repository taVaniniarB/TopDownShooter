#pragma once
#include "CScene.h"
class CScene_Start :
    public CScene
{
public:
    // 가상함수임을 명시적으로 적어준다 (키워드 없어도 오버라이딩 된 가상함수긴 함!)
    virtual void Enter();
    virtual void Exit();
    virtual void update();

public:
    CScene_Start();
    ~CScene_Start();
};

