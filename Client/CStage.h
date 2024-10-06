#pragma once
class CStage
{
public:
	SINGLE(CStage);

private:
	// SceneMgr와 달리 모든 씬을 알고 있다
	vector<CScene*>		m_vScene;
	int					m_iScore;
	CWeapon*			m_pWeapon;

public:
	void AddScore(int _iScore) { m_iScore += _iScore; std::cout << "점수: " << m_iScore << "\n";
	}
	void SavePlayerWeapon(CWeapon* _pWeapon);
	CWeapon* GetPlayerWeapon() { return m_pWeapon; }

public:
	void ResetScore() { m_iScore = 0; }
};

