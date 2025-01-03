#include "pch.h"
#include "CWeapon.h"
#include "CKeyMgr.h"
#include "CCollider.h"
#include "CPathMgr.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CSound.h"
#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"


CWeapon::CWeapon()
	: m_fDelay(0.f)
	, m_fCurDelay(99.f)
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
		Vec2 vObjectPos = m_pOwner->GetPos();
		SetPos(vObjectPos + m_vOffsetPos);

		m_fCurDelay += fDT;
	}
}

void CWeapon::render(HDC _dc)
{
	// 벡터에서 회전 각도 계산 (단위 벡터이므로 ArcTan2 사용)
	//float angle = VectorToAngle(m_vAimDir);
	float angle = atan2(m_vAimDir.y, m_vAimDir.x) * (180.0f / 3.141592f);

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

void CWeapon::Save(FILE* _pFile)
{
	fwrite(&m_eFullWeaponType, sizeof(FULL_WEAPON_TYPE), 1, _pFile);

	Vec2 vPos = GetPos();
	fwrite(&vPos, sizeof(Vec2), 1, _pFile);
}

void CWeapon::Load(FILE* pFile)
{
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	pScene->AddObject(this, GROUP_TYPE::DROPPED_WEAPON);
}


void CWeapon::PlayAttackSound(wstring _strSoundName)
{
	CSound* pNewSound = nullptr;
	pNewSound = CResMgr::GetInst()->FindSound(_strSoundName);
	pNewSound->Play();
	pNewSound->SetVolume(30.f);
}