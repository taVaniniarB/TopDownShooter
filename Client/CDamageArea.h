#pragma once
#include "CObject.h"

class CWeapon;

class CDamageArea :
    public CObject
{
public:
    CDamageArea(Vec2 _vScale);
    ~CDamageArea();

private:
    float m_fHoldTime;
    float m_faccTime;
    Vec2    m_vDir;
    CWeapon* m_pOwner;

public:
    float GetHoldTime() { return m_fHoldTime; }
    void SetDir(Vec2 _vDir)
    {
        m_vDir = _vDir;
        m_vDir.Normalize();
    }
    void SetOwner(CWeapon* _pOwner) { m_pOwner = _pOwner; }
    CWeapon* GetOwner() { return m_pOwner; }
    FULL_WEAPON_TYPE GetOwnerWeaponType();

public:
    virtual void update();
    virtual void render(HDC _dc);

    CLONE(CDamageArea);

public:
    virtual void OnCollisionEnter(CCollider* _pOther);
};

