#pragma once
#include "CWeapon.h"

class CSound;

class CGun:
	public CWeapon

{
private:
	int     m_iMaxMissile;		// �ִ� �Ѿ� ��
	int		m_iRemainMissile;	// ���� �Ѿ� ��
	CSound* m_pEmptySound;		// �� źâ�� �� ȿ���� ���ҽ�


private:
	void SetMaxMissile(int _i) { m_iMaxMissile = _i; m_iRemainMissile = _i; }
	void CreateMissile();

private:
	virtual void update();

public:
	void Attack();

	CLONE(CGun);

public:
	CGun();
	~CGun();

	friend class CWeaponFactory;
};

