#pragma once

class CMonster;

class CMonFactory
{
public:
	// ��ü ���� ȣ�� ������ "���� ��� �Լ�"
	static CMonster* CreateMonster(MON_TYPE _eType, Vec2 _vPos, FULL_WEAPON_TYPE _eWeapon);

public:
	CMonFactory() {}
	~CMonFactory() {}
};

