#pragma once

class CWeapon;

class CWeaponFactory
{
public:
	static CWeapon* CreateWeapon(FULL_WEAPON_TYPE _eWeaponType);

public:
	CWeaponFactory() {}
	~CWeaponFactory() {}
};

