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
	float	m_fRadAngle; // �Ѿ��� ��簢 (Radiation angle)
	int		m_iPelletNum; // �� ���� ������ �Ѿ��� ����

public:
	void Attack();

private:
	void CreateMissile();
};

