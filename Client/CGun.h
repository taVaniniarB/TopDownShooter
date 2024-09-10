#pragma once
#include "CWeapon.h"

class CSound;

class CGun:
	public CWeapon

{
private:
	int     m_iMaxBullet;   // �ִ� �Ѿ� ��
	int		m_iCurBullet;	// ���� �Ѿ� ��

	// �� źâ�� �� ȿ���� ���ҽ�
	CSound* m_pEmptySound;

public:
	void update();
	void render();

private:
	void SetMaxBullet(int _i) { m_iMaxBullet = _i; m_iCurBullet = _i; }
	
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

