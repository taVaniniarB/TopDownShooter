#pragma once
#include "CScene.h"

class CScene_End :
    public CScene
{
public:
    CScene_End();
    ~CScene_End();

public:
    virtual void Enter();
    virtual void Exit();
    virtual void update();
    virtual void render(HDC _dc);

private:

private:
    void CreateUI();
};

