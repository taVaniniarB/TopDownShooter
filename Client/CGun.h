#pragma once
#include "CWeapon.h"


class CSound;

class CGun:
	public CWeapon

{
private:
	int     m_iMaxAmmo;		// �ִ� �Ѿ� ��
	int		m_iRemainAmmo;	// ���� �Ѿ� ��
	CSound* m_pEmptySound;		// �� źâ�� �� ȿ���� ���ҽ�
	GUN_TYPE m_eGunType;


private:
	void SetMaxMissile(int _i) { m_iMaxAmmo = _i; m_iRemainAmmo = _i; }
	
protected:
	virtual void CreateMissile();

public:
	void SetType(GUN_TYPE _eType) { m_eGunType = _eType; }
	int GetAmmoNum() { return m_iRemainAmmo; }

public:
	virtual void Attack();
	void PlayShotSound(wstring _strSoundName);

	//CLONE(CGun);

public:
	CGun();
	~CGun();

	friend class CWeaponFactory;
};

