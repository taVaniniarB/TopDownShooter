#pragma once
#include "CObject.h"

struct tMonInfo
{
    float   fSpeed;       // 초당 이동할 픽셀 수
    int     iHP;          // 체력
    float   fRecogRange;  // 인지 범위
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
    const tMonInfo& GetInfo() { return m_tInfo; } // 구조체 크기가 크므로 레퍼런스 사용

private:
    // 아무나 몬스터 스탯을 바꾸면 안 되기 때문에 private
    // 대신 몬스터팩토리와 friend 클래스
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

