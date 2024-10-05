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
	GUN_TYPE m_eGunType;


private:
	void SetMaxMissile(int _i) { m_iMaxMissile = _i; m_iRemainMissile = _i; }
	void CreateMissile();

public:
	void SetType(GUN_TYPE _eType) { m_eGunType = _eType; }

public:
	void Attack();

	CLONE(CGun);

public:
	CGun();
	~CGun();

	friend class CWeaponFactory;
};

