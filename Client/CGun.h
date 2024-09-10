#pragma once
#include "CWeapon.h"

class CSound;

class CGun:
	public CWeapon

{
private:
	int     m_iMaxMissile;   // 최대 총알 수
	int		m_iCurMissile;	// 남은 총알 수

	// 빈 탄창일 때 효과음 리소스
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

