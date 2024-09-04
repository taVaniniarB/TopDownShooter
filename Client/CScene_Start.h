#pragma once
#include "CScene.h"
class CScene_Start :
    public CScene
{
private:
    bool m_bUseForce;
    float m_fForceRadius; // 힘 반경 (영향권)
    float m_fCurRadius;
    float m_fForce; // 힘의 크기
    Vec2 m_vForcePos; // 힘 발생 위치

public:
    // 가상함수임을 명시적으로 적어준다 (키워드 없어도 오버라이딩 된 가상함수긴 함!)
    virtual void Enter();
    virtual void Exit();
    virtual void update();
    virtual void render(HDC _dc);

public:
    void CreateForce();

public:
    CScene_Start();
    ~CScene_Start();
};

