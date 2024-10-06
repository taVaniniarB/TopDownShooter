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

//
//private:
//    CTexture* m_pTex;
//    int m_iImgIdx;
};

