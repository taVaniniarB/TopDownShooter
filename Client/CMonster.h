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
class CHitbox;

class CMonster :
    public CObject
{
private:
    tMonInfo m_tInfo;
    AI*    m_pAI;
    CWeapon* m_pWeapon;
    MON_TYPE m_eType;
    CHitbox* m_pHitbox;

public:
    float GetSpeed() { return m_tInfo.fSpeed; }
    void SetSpeed(float _f) { m_tInfo.fSpeed = _f; }
    void SetAI(AI* _AI);
    void SetType(MON_TYPE _type) { m_eType = _type; }
    MON_TYPE GetType() { return m_eType; }
    const tMonInfo& GetInfo() { return m_tInfo; } // ����ü ũ�Ⱑ ũ�Ƿ� ���۷��� ���

public:
    void SetWeapon(CWeapon* _pWeapon);
    void SetHitbox(CHitbox* _pHitbox);
    void DropWeapon();
    void SubHP();

public:
    void Save(FILE* _pFile);
    void Load(FILE* _pFile);

private:
    // �ƹ��� ���� ������ �ٲٸ� �� �Ǳ� ������ private
    // ��� �������丮�� friend Ŭ����
    void SetMonInfo(const tMonInfo& _info) { m_tInfo = _info; }

public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    
public:
    void update();
    void render(HDC _dc);
    CLONE(CMonster);

public:
    CMonster();
    ~CMonster();

    friend class CMonFactory;
};

