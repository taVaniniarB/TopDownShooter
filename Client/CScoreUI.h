#pragma once
#include "CTextUI.h"
class CScoreUI :
    public CTextUI
{
public:
    CScoreUI();
    ~CScoreUI();

public:
    void update();
    void render(HDC _dc);

private:
    int m_iPrevScore;
};

