#pragma once

class CMonster;

class CMonFactory
{
public:
	// ��ü ���� ȣ�� ������ "���� ��� �Լ�"
	static CMonster* CreateMonster(MON_TYPE _eType, Vec2 _vPos);

public:
	CMonFactory() {}
	~CMonFactory() {}
};

