#pragma once
#include "CWeapon.h"
class CMelee :
    public CWeapon
{
private:
    float m_fAttackRange;
    MELEE_TYPE m_eType;

public:
    void update();
    void render();

    CLONE(CMelee);

private:
    void Attack();

public:
    void SetType(MELEE_TYPE _eType) { m_eType = _eType; }

public:
    CMelee();
    ~CMelee();
};

