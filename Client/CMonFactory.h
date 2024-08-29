#pragma once

class CMonster;

enum class MON_TYPE
{
	NORMAL,
	RANGE,
};

class CMonFactory
{
public:
	// ��ü ���� ȣ�� ������ "���� ��� �Լ�"
	static CMonster* CreateMonster(MON_TYPE _eType, Vec2 _vPos);

public:
	CMonFactory() {}
	~CMonFactory() {}
};

