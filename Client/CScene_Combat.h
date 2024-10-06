#pragma once
#include "CScene.h"
class CScene_Combat :
    public CScene
{
private:
    //bool m_bUseForce;
    //float m_fForceRadius; // �� �ݰ� (�����)
    //float m_fCurRadius;
    //float m_fForce; // ���� ũ��
    //Vec2 m_vForcePos; // �� �߻� ��ġ
    wstring m_wSceneRelativePath;

private:
    int m_iCombo;

    float m_fComboTime; // �޺�Ÿ�� ���� �����ؾ� �޺��� ��������.
    float m_fCurTime;	// �ֱ��� �� óġ�κ��� �帥 �ð�

public:
    // �����Լ����� ��������� �����ش� (Ű���� ��� �������̵� �� �����Լ��� ��!)
    virtual void Enter();
    virtual void Exit();
    virtual void update();
    virtual void render(HDC _dc);

    void SetFile(const wstring& _wSceneRelativePath) { m_wSceneRelativePath = _wSceneRelativePath; }
//
//public:
//    void CreateForce();
public:
    int GetCombo() { return m_iCombo; }

private:
    void AddScore();
    void AddCombo();

    
    void ResetCombo() { m_iCombo = 0; }

public:
    CScene_Combat(const wstring& _wSceneRelativePath);
    ~CScene_Combat();
};

