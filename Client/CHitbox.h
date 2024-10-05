#pragma once
#include "CObject.h"
class CHitbox :
    public CObject
{
private:
    CObject* m_pOwner;

public:
    void SetOwner(CObject* _pOwner) { m_pOwner = _pOwner; }

public:
    virtual void update();
    virtual void render(HDC _dc);
    CLONE(CHitbox);

public:
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionEnter(CCollider* _pOther);

public:
    CHitbox();
    ~CHitbox();

};

