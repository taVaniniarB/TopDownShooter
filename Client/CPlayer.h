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

class CPlayer :
    public CObject
{
private:
    // vector<CObject*> m_vColObj;

    PLAYER_STATE m_eCurState;
    PLAYER_STATE m_ePrevState;
    int m_iDir;
    int m_iPrevDir;

public:
    virtual void update();
    virtual void render(HDC _dc);

private:
    void CreateMissile();
    void update_state();
    void update_move();
    void update_attack();
    void update_animation();
    void update_gravity();

    virtual void OnCollisionEnter(CCollider* _pOther);

    CLONE(CPlayer);

public:
    CPlayer();

    CPlayer(const CPlayer& _origin)
        : CObject(_origin)
    {}

    ~CPlayer();
};