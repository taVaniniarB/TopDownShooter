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

