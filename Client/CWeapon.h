#pragma once
#include "CObject.h"

enum class WEAPON_STATUS
{
	HOLD,
	DROPPED,
};

class CSound;

class CWeapon :
	public CObject
{
private:
	float m_fDelay;
	float m_fCurDelay;
	// ���� �� ȿ���� ���ҽ�
	CSound* m_pAttackSound;
	// ������ �� �� ȿ���� ���ҽ�
	CSound* m_pGetSound;
	WEAPON_STATUS m_eWeaponStatus; // ��� �ִ� �������� �ٴڿ� ����������
	
	CObject* m_pOwner;
	Vec2 m_vAimDir; // ���ع���
	Vec2 m_vPrevAimDir; // ���� ������ ���ع���


public:
	void update();
	void render();

public:
	void SetDelay(float _f) { m_fDelay = _f; }
	void SetOnwer(CObject* _pOwner) { m_pOwner = _pOwner; }
	void Drop();

	float GetDelay() { return m_fDelay; }
	float GetCurDelay() { reutrn m_fCurDelay; }
	void SetCurDelay(float _f) { m_fCurDelay = _f; }


private:
	void SetAimDir();


public:
	virtual void Attack() = 0;

public:
	CWeapon();
	~CWeapon();
};

