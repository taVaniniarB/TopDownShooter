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
	void AddScore(int _iScore); // ���͸� ó���� �� ȣ��
	void SaveScore() { m_iAccSceneScore = m_iScore; } // �ش� Scene Clear �� ȣ��
	void SetSavedScore() { m_iScore = m_iAccSceneScore; }

	const int& GetScore() { return m_iScore; }
	void SavePlayerWeapon(CWeapon* _pWeapon);

	CWeapon* GetPlayerWeapon() { return m_pWeapon; }

public:
	void ResetScore() { m_iScore = 0; }
};

