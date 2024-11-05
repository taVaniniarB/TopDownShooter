#pragma once
class CStage
{
public:
	SINGLE(CStage);

private:
	vector<CScene*>		m_vScene;
	int					m_iScore;
	int					m_iAccSceneScore;
	CWeapon*			m_pWeapon;

public:
	void AddScore(int _iScore); // 몬스터를 처리할 때 호출
	void SaveScore() { m_iAccSceneScore = m_iScore; } // 해당 Scene Clear 시 호출
	void SetSavedScore() { m_iScore = m_iAccSceneScore; }

	const int& GetScore() { return m_iScore; }
	void SavePlayerWeapon(CWeapon* _pWeapon);

	CWeapon* GetPlayerWeapon() { return m_pWeapon; }

public:
	void ResetScore() { m_iScore = 0; }
};

