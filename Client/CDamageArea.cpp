#include "pch.h"
#include "CDamageArea.h"
#include "CCollider.h"
#include "CTimeMgr.h"
#include "CWeapon.h"

CDamageArea::CDamageArea(Vec2 _vScale)
	: m_fHoldTime(0.1f)
	, m_faccTime(0.f)
{
	CreateCollider();
	GetCollider()->SetScale(_vScale);
}

CDamageArea::~CDamageArea()
{
}

FULL_WEAPON_TYPE CDamageArea::GetOwnerWeaponType()
{
	return m_pOwner->GetFullWeaponType();
}

void CDamageArea::update()
{
	m_faccTime += fDT;

	if (m_fHoldTime < m_faccTime)
		DeleteObject(this);
}

void CDamageArea::render(HDC _dc)
{
	component_render(_dc);
}

void CDamageArea::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pObject = _pOther->GetObj();
	if (L"Hitbox_Monster" == pObject->GetName()
		|| L"Wall" == pObject->GetName()
		|| L"Hitbox_Player" == pObject->GetName())
	{
		if (L"Hitbox_Monster" == pObject->GetName())
			CCamera::GetInst()->Shake(300.f, m_vDir);
	}
}
