#pragma once
#include "CObject.h"
class CUI :
    public CObject
{
private:

public:
    virtual void update();
    virtual void render(HDC _dc);

    CLONE(CUI);

public:
    CUI();
    ~CUI();
};

