#pragma once
class CStage
{
public:
	SINGLE(CStage);

private:
	vector<CScene*>		m_vScene;
	int					m_iScore;
	CWeapon*			m_pWeapon;

public:
	void AddScore(int _iScore);
	const int& GetScore() { return m_iScore; }
	void SavePlayerWeapon(CWeapon* _pWeapon);
	void SavePrevScore();

	CWeapon* GetPlayerWeapon() { return m_pWeapon; }

public:
	void ResetScore() { m_iScore = 0; }
};

