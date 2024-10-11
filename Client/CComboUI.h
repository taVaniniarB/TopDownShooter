#pragma once
#include "CTextUI.h"
class CComboUI :
    public CTextUI
{
public:
    CComboUI();
    ~CComboUI();

public:
    void render(HDC _dc);
};

