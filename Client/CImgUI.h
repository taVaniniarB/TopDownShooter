#pragma once
#include "CUI.h"
class CImgUI :
    public CUI
{
public:
    CImgUI();
    ~CImgUI();

    CLONE(CImgUI);

public:
    virtual void render(HDC _dc);
};

