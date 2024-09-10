#pragma once
#include "CWeapon.h"
class CMelee :
    public CWeapon
{
private:
    float m_fAttackRange;

public:
    void update();
    void render();
    
    CLONE(CMelee);

private:
    void Attack();

public:
    CMelee();
    ~CMelee();
};

