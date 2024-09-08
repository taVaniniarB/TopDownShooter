#pragma once
#include "CObject.h"



class CWall :
    public CObject
{
private:
    virtual void start();
    virtual void update();
    virtual void render(HDC _dc);
    
public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);
    CLONE(CWall);


public:
    virtual void Save(FILE* _pFile);
    virtual void Load(FILE* _pFile);


public:
    CWall();
    ~CWall();
};