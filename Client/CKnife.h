#pragma once
#include "CMelee.h"
class CKnife :
    public CMelee
{
public:
    CKnife();
    ~CKnife();

    CLONE(CKnife);

public:
    virtual void Attack();

private:
    //virtual void CreateDamageArea();
};

