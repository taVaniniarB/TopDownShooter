#pragma once
#include "CScene.h"
class CScene_Combat :
    public CScene
{
private:
    wstring m_wSceneRelativePath;

private:
    int m_iCombo;

    float m_fComboTime; // 콤보타임 내로 공격해야 콤보가 더해진다.
    float m_fCurTime;	// 최근의 적 처치로부터 흐른 시간

public:
    // 가상함수임을 명시적으로 적어준다 (키워드 없어도 오버라이딩 된 가상함수긴 함!)
    virtual void Enter();
    virtual void Exit();
    virtual void update();
    virtual void render(HDC _dc);

    void SetFile(const wstring& _wSceneRelativePath) { m_wSceneRelativePath = _wSceneRelativePath; }

public:
    int GetCombo() { return m_iCombo; }

private:
    void AddScore();
    void AddCombo();
    void ResetCombo() { m_iCombo = 0; }

    void CreateCombatSceneUI();
    void CreateCollisionGroup();

    void PlayerSetting(CObject* pPlayer);
    void CameraSetting(CObject* pPlayer);

public:
    CScene_Combat(const wstring& _wSceneRelativePath);
    ~CScene_Combat();
};

