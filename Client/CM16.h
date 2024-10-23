#pragma once
#include "CGun.h"
class CM16 :
    public CGun
{
public:
    CM16();
    ~CM16();

    CLONE(CM16);

public:
    void Attack();

private:
    void CreateMissile();
};

