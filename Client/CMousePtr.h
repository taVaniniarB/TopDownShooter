#pragma once
#include "CObject.h"
class CMousePtr :
    public CObject
{
public:
    CMousePtr();
    ~CMousePtr();

public:
    virtual void update();
    virtual void render(HDC _dc);

public:
    CLONE(CMousePtr);
};

