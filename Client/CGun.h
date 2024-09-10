#pragma once
#include "CWeapon.h"

class CSound;

class CGun:
	public CWeapon

{
private:
	int     m_iMaxMissile;		// 최대 총알 수
	int		m_iRemainMissile;	// 남은 총알 수
	CSound* m_pEmptySound;		// 빈 탄창일 때 효과음 리소스


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

