#pragma once
class CStage
{
public:
	SINGLE(CStage);

private:
	// SceneMgr�� �޸� ��� ���� �˰� �ִ�
	vector<CScene*>		m_vScene;
	int					m_iScore;
	CWeapon*			m_pWeapon;

public:
	void AddScore(int _iScore) { m_iScore += _iScore; std::cout << "����: " << m_iScore << "\n";
	}
	void SavePlayerWeapon(CWeapon* _pWeapon);
	CWeapon* GetPlayerWeapon() { return m_pWeapon; }

public:
	void ResetScore() { m_iScore = 0; }
};

