#pragma once
#include "CObject.h"
class CMissile :
    public CObject
{
private:
    float   m_fTheta; // �̵� ���� ��
    Vec2    m_vDir;   // ���ͷ� ��Ÿ���� ��
    Image* m_pImage;

public:
    void SetDir(float _fTheta) { m_fTheta = _fTheta; }
    void SetDir(Vec2 _vDir)
    {
        m_vDir = _vDir;
        m_vDir.Normalize();
    }
    void SetImage(wstring _imageName, wstring _imagePath);

public:
    virtual void update();
    virtual void render(HDC _dc);
    virtual void start();

public:
    virtual void OnCollisionEnter(CCollider* _pOther);

    CLONE(CMissile);


public:
    CMissile();
    ~CMissile();
};

