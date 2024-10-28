#pragma once
#include "CScene.h"

class CScene_Main :
    public CScene
{
public:
    CScene_Main();
    ~CScene_Main();

public:
    virtual void Enter();
    virtual void Exit();
    virtual void update();
    virtual void render(HDC _dc);

private:

private:
    void CreateUI();
};

