#include "pch.h"
#include "CWeapon.h"
#include "CKeyMgr.h"
#include "CCollider.h"
#include "CTexture.h"

CWeapon::CWeapon()
	: m_fDelay(0.f)
	, m_fCurDelay(0.f)
	, m_pOwner(nullptr)
	, m_pGetSound(nullptr)
	, m_pAttackSound(nullptr)
	, m_eWeaponStatus(WEAPON_STATUS::HOLD)
{
	m_vOffsetPos = Vec2(0.f, 3.f);
	CreateCollider();
	SetName(L"Weapon");
}

CWeapon::~CWeapon()
{
}

void CWeapon::start()
{
	Vec2 vScale = GetScale();
	GetCollider()->SetScale(vScale);
}

void CWeapon::update()
{
	if (WEAPON_STATUS::HOLD == m_eWeaponStatus)
	{
		SetAimDir();

		Vec2 vObjectPos = m_pOwner->GetPos();
		SetPos(vObjectPos + m_vOffsetPos);
	}
}

void CWeapon::render(HDC _dc)
{
	// 각도(AimDir)에 따라 회전
	//m_vFinalPos;
	m_vAimDir;

	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	CTexture* pTex = GetTexture();
	Vec2 vScale = GetScale();

	// dc, 출력될 위치(오브젝트가 출력될 좌상단 위치), 가로세로길이, 텍스처의 DC, 텍스처 내에서 자를 위치, 가로세로길이
	TransparentBlt(_dc
		, (int)(vPos.x - vScale.x / 2.f)
		, (int)(vPos.y - vScale.y / 2.f)
		, (int)(vScale.x)
		, (int)(vScale.y)
		, pTex->GetDC()
		, (int)(0) // 이미지 상 좌상단좌표 x,y
		, (int)(0)
		, (int)(vScale.x)
		, (int)(vScale.y)
		, RGB(0, 255, 0));

	component_render(_dc);
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
