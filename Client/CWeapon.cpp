#include "pch.h"
#include "CWeapon.h"
#include "CKeyMgr.h"

CWeapon::CWeapon()
	: m_fDelay(0.f)
	, m_fCurDelay(0.f)
	, m_pOwner(nullptr)
	, m_pGetSound(nullptr)
	, m_pAttackSound(nullptr)
	, m_eWeaponStatus(WEAPON_STATUS::DROPPED)
{
	CreateCollider();
}

CWeapon::~CWeapon()
{
}

void CWeapon::update()
{
	SetAimDir();
}

void CWeapon::render()
{
}

void CWeapon::Drop()
{
	m_eWeaponStatus = WEAPON_STATUS::DROPPED;
	m_vAimDir = Vec2(1.f, 0.f);
	m_vPrevAimDir = Vec2(1.f, 0.f);
	m_pOwner = nullptr;
}

void CWeapon::SetAimDir()
{
	m_vPrevAimDir = m_vAimDir;

	Vec2 vMousePos = MOUSE_POS;
	CCamera::GetInst()->GetRealPos(vMousePos);

	Vec2 vPlayerPos = GetPos();

	// 플레이어 > 마우스 향하는 벡터
	Vec2 vAimVec = vMousePos - vPlayerPos;
	if (vAimVec.isZero())
	{
		m_vAimDir = m_vPrevAimDir;
		return;
	}

	m_vAimDir = vAimVec.Nomalize();
}
