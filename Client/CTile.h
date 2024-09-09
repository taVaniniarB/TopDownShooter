#pragma once
#include "CObject.h"

class CTexture;


class CTile :
    public CObject
{
private:
    CTexture* m_pTileTex;
    // int ��� ����: �ε����� -1�̸� �ƹ��͵� �������� �ʰڴٴ� �ǹ̷� ������
    int m_iImgIdx;

public:
    void SetTexture(CTexture* _pTex)
    {
        m_pTileTex = _pTex;
    }

    void AddImgIdx()
    {
        ++m_iImgIdx;
    }
    void SetImgIdx(int _idx)
    {
        m_iImgIdx = _idx;
    }

private:
    virtual void update();
    virtual void render(HDC _dc);

public:
    virtual void Save(FILE* _pFile);
    virtual void Load(FILE* _pFile);

    CLONE(CTile);

public:
    CTile();
    ~CTile();
};

