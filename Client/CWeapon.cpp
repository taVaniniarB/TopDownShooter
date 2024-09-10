#include "pch.h"
#include "CWeapon.h"
#include "CKeyMgr.h"
#include "CCollider.h"

CWeapon::CWeapon()
	: m_fDelay(0.f)
	, m_fCurDelay(0.f)
	, m_pOwner(nullptr)
	, m_pGetSound(nullptr)
	, m_pAttackSound(nullptr)
	, m_eWeaponStatus(WEAPON_STATUS::DROPPED)
{
	m_vOffsetPos = Vec2(0.f, 3.f);
	CreateCollider();
}

CWeapon::~CWeapon()
{
}

void CWeapon::finalUpdate()
{
	SetAimDir();

	Vec2 vObjectPos = m_pOwner->GetPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;

}

void CWeapon::render()
{
	// 각도(AimDir)에 따라 회전
	m_vFinalPos;
	m_vAimDir;
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
	vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);

	Vec2 vPlayerPos = m_pOwner->GetPos();
	//CCamera::GetInst()->GetInst()->GetRenderPos(vPlayerPos);

	// 플레이어 > 마우스 향하는 벡터
	Vec2 vAimVec = vMousePos - vPlayerPos;
	if (vAimVec.isZero())
	{
		m_vAimDir = m_vPrevAimDir;
		return;
	}

	m_vAimDir = vAimVec.Nomalize();
}
