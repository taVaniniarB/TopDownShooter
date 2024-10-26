#pragma once
#include "CWeapon.h"
class CMelee :
    public CWeapon
{
private:
    MELEE_TYPE m_eType;


protected:
    virtual void CreateDamageArea();

public:
    virtual void Attack();

public:
    void SetType(MELEE_TYPE _eType) { m_eType = _eType; }

public:
    CMelee();
    ~CMelee();
};

