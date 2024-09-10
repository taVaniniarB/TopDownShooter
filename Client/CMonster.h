#pragma once
#include "CObject.h"

struct tMonInfo
{
    float   fSpeed;       // �ʴ� �̵��� �ȼ� ��
    int     iHP;          // ü��
    float   fRecogRange;  // ���� ����
};

class AI;
class CWeapon;

class CMonster :
    public CObject
{
private:
    tMonInfo m_tInfo;
    AI*    m_pAI;
    CWeapon* m_pWeapon;

public:
    float GetSpeed() { return m_tInfo.fSpeed; }
    void SetSpeed(float _f) { m_tInfo.fSpeed = _f; }
    void SetAI(AI* _AI);
    const tMonInfo& GetInfo() { return m_tInfo; } // ����ü ũ�Ⱑ ũ�Ƿ� ���۷��� ���

private:
    // �ƹ��� ���� ������ �ٲٸ� �� �Ǳ� ������ private
    // ��� �������丮�� friend Ŭ����
    void SetMonInfo(const tMonInfo& _info) { m_tInfo = _info; }

public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    
public:
    void update();
    CLONE(CMonster);

public:
    CMonster();
    ~CMonster();

    friend class CMonFactory;
};

