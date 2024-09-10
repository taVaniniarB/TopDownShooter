#pragma once
#include "CWeapon.h"

class CSound;

class CGun:
	public CWeapon

{
private:
	int     m_iMaxBullet;   // 최대 총알 수
	int		m_iCurBullet;	// 남은 총알 수

	// 빈 탄창일 때 효과음 리소스
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

