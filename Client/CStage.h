#pragma once
class CStage
{
public:
	CStage();
	~CStage();

private:
	vector<CScene*>		m_vScene;
	int					m_iScore;

public:
	void AddScore(int _iScore) { m_iScore += _iScore; }
};

