#pragma once
#include "CUI.h"
class CTextUI :
    public CUI
{
public:
    CTextUI();
    ~CTextUI();

    CLONE(CTextUI);

private:
    wstring m_strText;
};

