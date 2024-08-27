#pragma once
#include "CRes.h"
class CTexture :
    public CRes
{
private:
    HDC         m_dc;
    HBITMAP     m_hBit;
    BITMAP      m_bitInfo;    //��Ʈ���� ������ �����ϴ� ����ü BITMAP

public:
    void Load(const wstring& _strFilePath);
    void Create(UINT _iwidth, UINT _iHeight);

    UINT Width() { return m_bitInfo.bmWidth; }
    UINT Height() { return m_bitInfo.bmHeight; }

    HDC GetDC() { return m_dc; }

private: // �ؽ�ó�� �ƹ��� ���� ���ϰ� �Ѵ�
    // ���� ���ҽ� �Ŵ����� ���� ����
    CTexture();
    virtual ~CTexture();

    friend class CResMgr;
};

