#pragma once
#include "CTextUI.h"
class CAmmoUI :
    public CTextUI
{
public:
    CAmmoUI();
    ~CAmmoUI();

public:
    void render(HDC _dc);
};

