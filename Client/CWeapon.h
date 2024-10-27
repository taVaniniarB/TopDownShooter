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
	WEAPON_TYPE m_eWeaponType;
	FULL_WEAPON_TYPE m_eFullWeaponType;

	float m_fDelay;
	float m_fCurDelay;
	float m_fAttackRange;

	// 공격 시 효과음 리소스
	CSound* m_pAttackSound;
	// 장착할 때 때 효과음 리소스
	CSound* m_pGetSound;
	WEAPON_STATUS m_eWeaponStatus; // 들고 있는 무기인지 바닥에 떨어진건지
	
	CObject* m_pOwner;
	Vec2 m_vAimDir; // 조준방향
	Vec2 m_vPrevAimDir; // 이전 프레임 조준방향

	Vec2 m_vPos;
	Vec2 m_vOffsetPos;

	Image* m_pImage;


public:
	virtual void start();
	virtual void update();
	virtual void render(HDC _dc);

public:
	void SetDelay(float _f) { m_fDelay = _f; m_fCurDelay = _f; }
	float GetAttackRange() { return m_fAttackRange; }
	void SetAttackRange(float _f) { m_fAttackRange = _f; }
	CObject* GetOwner() { return m_pOwner; }
	void SetOnwer(CObject* _pOwner) { m_pOwner = _pOwner; }
	void SetWeaponType(WEAPON_TYPE _eType) { m_eWeaponType = _eType; }
	void SetFullWeaponType(FULL_WEAPON_TYPE _eType) { m_eFullWeaponType = _eType; }
	WEAPON_TYPE GetWeaponType() { return m_eWeaponType; }
	FULL_WEAPON_TYPE GetFullWeaponType() { return m_eFullWeaponType; }

	void Drop();

	float GetDelay() { return m_fDelay; }
	float GetCurDelay() { return m_fCurDelay; }
	void SetCurDelay(float _f) { m_fCurDelay = _f; }

	WEAPON_STATUS GetStatus() { return m_eWeaponStatus; }
	void SetStatus(WEAPON_STATUS _eStatus) { m_eWeaponStatus = _eStatus; }

	Vec2 GetAimDir() { return m_vAimDir; }
	void SetOwner(CObject* _pOwner) { m_pOwner = _pOwner; }

	//void OncollisionEnter(CCollider* _pOther);
	void SetImage(Image* _pImage);

	void SetAimDir(Vec2 _vAimDir) { m_vAimDir = _vAimDir; }

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);


public:
	virtual void Attack() = 0;
	void PlayAttackSound(wstring strSoundName);

public:
	CWeapon();
	~CWeapon();
};

