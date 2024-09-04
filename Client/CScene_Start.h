#pragma once
#include "CScene.h"
class CScene_Start :
    public CScene
{
private:
    bool m_bUseForce;
    float m_fForceRadius; // �� �ݰ� (�����)
    float m_fCurRadius;
    float m_fForce; // ���� ũ��
    Vec2 m_vForcePos; // �� �߻� ��ġ

public:
    // �����Լ����� ��������� �����ش� (Ű���� ��� �������̵� �� �����Լ��� ��!)
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

