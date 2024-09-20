#pragma once
#include "CScene.h"
class CCombatScene :
    public CScene
{
private:
	int m_iScore;
	int m_iCombo;

	float m_fComboTime; // �޺�Ÿ�� ���� �����ؾ� �޺��� ��������.
	float m_fCurTime;	// �ֱ��� �� óġ�κ��� �帥 �ð�

public:
	virtual void update();

private:
	int GetScore() { return m_iScore; }
	int GetCombo() { return m_iCombo; }

	void AddScore();
	void AddCombo();

	void ResetScore() { m_iScore = 0; }
	void ResetCombo() { m_iCombo = 0; }

public:
	CCombatScene();
	~CCombatScene();
};

