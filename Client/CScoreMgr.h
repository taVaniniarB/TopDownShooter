#pragma once
class CScoreMgr
{
	SINGLE(CScoreMgr);
private:
	int m_iScore;
	int m_iCombo;

	float m_fComboTime; // 콤보타임 내로 공격해야 콤보가 더해진다.
	float m_fCurTime;	// 최근의 적 처치로부터 흐른 시간

public:
	void update();
	
public:
	int GetScore() { return m_iScore; }
	int GetCombo() { return m_iCombo; }

	void AddScore();
	void AddCombo();

	void ResetScore() { m_iScore = 0; }
	void ResetCombo() { m_iCombo = 0; }
};

