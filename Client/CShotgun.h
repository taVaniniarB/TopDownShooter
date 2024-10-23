#pragma once
#include "CGun.h"
class CShotgun :
	public CGun
{
public:
	CShotgun();
	~CShotgun();

	CLONE(CShotgun);

private:
	float	m_fRadAngle; // 총알의 방사각 (Radiation angle)
	int		m_iPelletNum; // 한 번에 나가는 총알의 개수

public:
	void Attack();

private:
	void CreateMissile();
};

