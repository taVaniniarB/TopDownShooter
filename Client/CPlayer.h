#pragma once
#include "CObject.h"


enum class PLAYER_STATE
{
    IDLE,
    WALK,
    RUN,
    ATTACK,
    DEAD,
    JUMP,
};

// 공격 상태 중에서도 세부 상태
enum class PLAYER_ATTACK
{
    NOMAL_ATT_1,
    NOMAL_ATT_2,
    NOMAL_ATT_3,
    NOMAL_ATT_4,

    SKILL_ATT_1,
    // ...
};

class CTexture;
class CWeapon;
class CHitbox;

class CPlayer :
    public CObject
{
private:
    PLAYER_STATE m_eCurState;
    PLAYER_STATE m_ePrevState;
    int m_iDir; // 움직이는 방향
    int m_iPrevDir;
    float m_fSpeed;

    int m_iHP;

    CWeapon* m_pWeapon;
    Vec2 m_vAimDir;
    CHitbox* m_pHitbox;

public:
    virtual void update();
    virtual void render(HDC _dc);

    CWeapon* GetWeapon() { return m_pWeapon; }

public:
    void SetWeapon(CWeapon* _pWeapon);
    void SetHitbox(CHitbox* _pHitbox);
    void SetHP(int _iHP) { m_iHP = _iHP; }
    int GetHP() { return m_iHP; }
    void SubHP(wstring _strWeaponName);

    void PlayerDeath(std::wstring& _strWeapon);
    
private:
    void DropWeapon();
    void ExchangeWeapon(CWeapon* _pWeapon);
    void PickupWeapon(CWeapon* _pWeapon);
    void SetAimDir();

private:
    void update_state();
    void update_move();
    void update_attack();
    void update_animation();

public:
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionEnter(CCollider* _pOther);

public:
    void Save(FILE* _pFile);
    void Load(FILE* _pFile);

    CLONE(CPlayer);

public:
    CPlayer();

    CPlayer(const CPlayer& _origin)
        : CObject(_origin)
        , m_eCurState(PLAYER_STATE::IDLE)
        , m_ePrevState(PLAYER_STATE::IDLE)
        , m_iDir(0)
        , m_iPrevDir(0)
        , m_iHP(0)
        , m_pHitbox(nullptr)
        , m_pWeapon(nullptr)
    {}

    ~CPlayer();
};