#pragma once
#include "CScene.h"
class CScene_Story :
    public CScene
{
public:
    CScene_Story();
    ~CScene_Story();

public:
    virtual void Enter();
    virtual void Exit();
    virtual void update();
    virtual void render();
   
};

