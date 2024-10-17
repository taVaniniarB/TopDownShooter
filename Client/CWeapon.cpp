#include "pch.h"
#include "CWeapon.h"
#include "CKeyMgr.h"
#include "CCollider.h"
#include "CPathMgr.h"
#include "CTexture.h"

CWeapon::CWeapon()
	: m_fDelay(0.f)
	, m_fCurDelay(0.f)
	, m_pOwner(nullptr)
	, m_pGetSound(nullptr)
	, m_pAttackSound(nullptr)
	, m_eWeaponStatus(WEAPON_STATUS::HOLD)
	, m_pImage(nullptr)
	, m_eWeaponType(WEAPON_TYPE::MELEE)
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
	// ���Ϳ��� ȸ�� ���� ��� (���� �����̹Ƿ� ArcTan2 ���)
	//float angle = VectorToAngle(m_vAimDir);
	float angle = atan2(m_vAimDir.y, m_vAimDir.x) * (180.0f / 3.141592f);

	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	CTexture* pTex = GetTexture();
	Vec2 vScale = GetScale();

	// GDI+�� Graphics ��ü ����
	Gdiplus::Graphics graphics(_dc);

	// ȸ���� �������� �̹��� �߽����� ����
	int centerX = (int)(vPos.x);
	int centerY = (int)(vPos.y);
	// GDI+ ��ȯ ����: �߽����� �̵� -> ȸ�� -> ���� ��ġ�� ���ƿ�
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

void CWeapon::Drop()
{
	m_eWeaponStatus = WEAPON_STATUS::DROPPED;
	m_vAimDir = Vec2(1.f, 0.f);
	m_vPrevAimDir = Vec2(1.f, 0.f);
	m_pOwner = nullptr;
}


void CWeapon::SetImage(Image* _pImage)
{
	m_pImage = _pImage;
}

void CWeapon::SetAimDir()
{
	m_vPrevAimDir = m_vAimDir;
	Vec2 vAimVec = {};

	if (m_pOwner->GetName() == L"Player")
	{
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);

		Vec2 vPlayerPos = m_pOwner->GetPos();
		//CCamera::GetInst()->GetInst()->GetRenderPos(vPlayerPos);

		// �÷��̾� > ���콺 ���ϴ� ����
		vAimVec = vMousePos - vPlayerPos;
	}
	
	if (vAimVec.isZero())
	{
		m_vAimDir = m_vPrevAimDir;
		return;
	}

	m_vAimDir = vAimVec.Normalize();
}
