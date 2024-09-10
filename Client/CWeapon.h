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
	// 공격 시 효과음 리소스
	CSound* m_pAttackSound;
	// 장착할 때 때 효과음 리소스
	CSound* m_pGetSound;
	WEAPON_STATUS m_eWeaponStatus; // 들고 있는 무기인지 바닥에 떨어진건지
	
	CObject* m_pOwner;
	Vec2 m_vAimDir; // 조준방향
	Vec2 m_vPrevAimDir; // 이전 프레임 조준방향

	Vec2 m_vFinalPos;
	Vec2 m_vOffsetPos;


public:
	void finalUpdate();
	void render();

public:
	void SetDelay(float _f) { m_fDelay = _f; m_fCurDelay = _f; }
	void SetOnwer(CObject* _pOwner) { m_pOwner = _pOwner; }
	void Drop();

	float GetDelay() { return m_fDelay; }
	float GetCurDelay() { return m_fCurDelay; }
	void SetCurDelay(float _f) { m_fCurDelay = _f; }

	Vec2 GetFinalPos() { return m_vFinalPos; }
	Vec2 GetAimDir() { return m_vAimDir; }

	void SetOwner(CObject* _pOwner) { m_pOwner = _pOwner; }

private:
	void SetAimDir();


public:
	virtual void Attack() = 0;

public:
	CWeapon();
	~CWeapon();
};

