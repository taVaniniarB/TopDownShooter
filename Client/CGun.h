#pragma once
#include "CWeapon.h"

class CSound;

class CGun:
	public CWeapon

{
private:
	int     m_iMaxMissile;   // �ִ� �Ѿ� ��
	int		m_iCurMissile;	// ���� �Ѿ� ��

	// �� źâ�� �� ȿ���� ���ҽ�
	CSound* m_pEmptySound;

public:
	void update();
	void render();

private:
	void SetMaxMissile(int _i) { m_iMaxMissile = _i; m_iCurMissile = _i; }
	
private:
	void CreateMissile();

public:
	void Attack();

	CLONE(CGun);

public:
	CGun();
	~CGun();

	friend class CWeaponFactory;
};

