#pragma once
#include "CRes.h"
class CTexture :
    public CRes
{
private:
    HDC         m_dc;
    HBITMAP     m_hBit;
    BITMAP      m_bitInfo;    //비트맵의 정보를 저장하는 구조체 BITMAP

public:
    void Load(const wstring& _strFilePath);

    UINT Width() { return m_bitInfo.bmWidth; }
    UINT Height() { return m_bitInfo.bmHeight; }

    HDC GetDC() { return m_dc; }

private: // 텍스처는 아무나 생성 못하게 한다
    // 오직 리소스 매니저만 생성 가능
    CTexture();
    virtual ~CTexture();

    friend class CResMgr;
};

