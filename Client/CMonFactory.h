#pragma once

class CMonster;

class CMonFactory
{
public:
	// 객체 없이 호출 가능한 "정적 멤버 함수"
	static CMonster* CreateMonster(MON_TYPE _eType, Vec2 _vPos, FULL_WEAPON_TYPE _eWeapon);

public:
	CMonFactory() {}
	~CMonFactory() {}
};

