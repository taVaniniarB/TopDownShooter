#pragma once
#include "CImgUI.h"
class CRestartUI :
    public CImgUI
{
public:
    CRestartUI();
    ~CRestartUI();

private:
    Image*  m_pBackground;
    Image*  m_pImg;
    float   m_fCurTime;
    float   m_fAmplitude;    // 최대 각도
    float   m_fFrequency;     // 주기
    float   m_fAngle;

public:
    virtual void update();
    virtual void render(HDC _dc);
};

