#pragma once

class CWeapon;

enum class WEAPON_TYPE
{
	MELEE,
	GUN,
};

enum class MELEE_TYPE
{
	KNIFE,
	PIPE,
	FIST,
};

enum class GUN_TYPE
{
	M16,
	SHOTGUN,
};

class CWeaponFactory
{
public:
	static CWeapon* CreateWeapon(WEAPON_TYPE _eWeaponType, MELEE_TYPE _eMeleeType, GUN_TYPE _eGunType);

public:
	CWeaponFactory() {}
	~CWeaponFactory() {}
};

