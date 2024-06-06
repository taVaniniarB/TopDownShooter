#pragma once
#include "CObject.h"
class CMissile :
    public CObject
{
private:
    float   m_fTheta; // 이동 방향 각

    Vec2    m_vDir;   // 벡터로 나타내는 각

public:
    void SetDir(float _fTheta) { m_fTheta = _fTheta; }
    void SetDir(Vec2 _vDir)
    {
        m_vDir = _vDir;
        m_vDir.Nomalize();
    }
public:
    virtual void update();
    virtual void render(HDC _dc);

public:
    virtual void OnCollisionEnter(CCollider* _pOther);

    CLONE(CMissile);

public:
    CMissile();
    ~CMissile();
};

