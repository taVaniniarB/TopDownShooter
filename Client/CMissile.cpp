#include "pch.h"
#include "CMissile.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "CCamera.h"
#include "CResMgr.h"

CMissile::CMissile()
	: m_fTheta(PI / 2.f)
	, m_vDir(Vec2(1.f, 1.f))
	, m_pImage(nullptr)
{
	m_vDir.Normalize();
	CreateCollider();
	GetCollider()->SetScale(Vec2(10.f, 10.f));
	SetName(L"Missile");

	Image* pImage = CResMgr::GetInst()->LoadGdiImage(L"ammo", L"texture\\weapon\\ammo.png");
	m_pImage = pImage;
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
	// 벡터에서 회전 각도 계산 (단위 벡터이므로 ArcTan2 사용)
	//float angle = VectorToAngle(m_vAimDir);
	float angle = atan2(m_vDir.y, m_vDir.x) * (180.0f / 3.141592f);

	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	CTexture* pTex = GetTexture();
	Vec2 vScale = GetScale();

	// GDI+의 Graphics 객체 생성
	Gdiplus::Graphics graphics(_dc);

	// 회전의 기준점을 이미지 중심으로 설정
	int centerX = (int)(vPos.x);
	int centerY = (int)(vPos.y);
	// GDI+ 변환 적용: 중심으로 이동 -> 회전 -> 원래 위치로 돌아옴
	graphics.TranslateTransform(centerX, centerY);
	graphics.RotateTransform(angle);
	graphics.TranslateTransform(-centerX, -centerY);


	graphics.DrawImage(m_pImage,
		(int)(vPos.x - vScale.x / 2.f),
		(int)(vPos.y - vScale.y / 2.f),
		(int)(vScale.x),
		(int)(vScale.y));

	//component_render(_dc);
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
