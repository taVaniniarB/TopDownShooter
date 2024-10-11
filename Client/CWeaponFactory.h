#pragma once

class CWeapon;

class CWeaponFactory
{
public:
	static CWeapon* CreateWeapon(WEAPON_TYPE _eWeaponType, MELEE_TYPE _eMeleeType, GUN_TYPE _eGunType);

public:
	CWeaponFactory() {}
	~CWeaponFactory() {}
};

