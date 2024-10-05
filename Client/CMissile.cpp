#include "pch.h"
#include "CMissile.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "CCamera.h"

CMissile::CMissile()
	: m_fTheta(PI / 2.f)
	, m_vDir(Vec2(1.f, 1.f))
{
	m_vDir.Nomalize();
	CreateCollider();
	GetCollider()->SetScale(Vec2(10.f, 10.f));
	SetName(L"Missile");
}

CMissile::~CMissile()
{
}

void CMissile::update()
{
	Vec2 vPos = GetPos();

	vPos.x += 2000.f * m_vDir.x * fDT;
	vPos.y += 2000.f * m_vDir.y * fDT;

	SetPos(vPos);
}

void CMissile::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);

	Vec2 vScale = GetScale();

	Ellipse(_dc, (int)(vPos.x - vScale.x / 2.f), (int)(vPos.y - vScale.y / 2.f)
		, (int)(vPos.x + vScale.x / 2.f), (int)(vPos.y + vScale.y / 2.f));

	component_render(_dc);
}

void CMissile::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pObject = _pOther->GetObj();
	if (L"Hitbox_Monster" == pObject->GetName()
		|| L"Wall" == pObject->GetName()
		|| L"Hitbox_Player" == pObject->GetName())
	{
		if (L"Hitbox_Monster" == pObject->GetName())
			CCamera::GetInst()->Shake(300.f, m_vDir);
		DeleteObject(this);
	}
}
