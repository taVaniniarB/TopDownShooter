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
    Vec2    m_vBGOffset;

public:
    virtual void update();
    virtual void render(HDC _dc);

public:
    void SetImage(Image* img) { m_pImg = img; }
    void SetBgImage(Image* img) { m_pBackground= img; }

    void SetBGOffset(Vec2 _vec) { m_vBGOffset = _vec; }
    void SetAmplitude(float _fAmp) { m_fAmplitude = _fAmp; }
};

