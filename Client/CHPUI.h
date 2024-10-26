#pragma once
#include "CImgUI.h"
class CHPUI :
    public CImgUI
{
public:
    CHPUI();
    ~CHPUI();

private:
    int m_iHP;
    float m_iDist;

public:
    virtual void render(HDC _dc);

public:
    void SetHPUINum(int _iNum) { m_iHP = _iNum; }

    friend class CPlayer;
};

