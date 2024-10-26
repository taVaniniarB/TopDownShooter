#include "pch.h"
#include "CGun.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CResMgr.h"
#include "CSound.h"
#include "CMissile.h"
#include "CTimeMgr.h"

CGun::CGun()
	: m_iMaxAmmo(0)
	, m_iRemainAmmo(0)
	, m_pEmptySound(nullptr)
{
}

CGun::~CGun()
{
}


void CGun::Attack()
{
	m_iRemainAmmo--;
	SetCurDelay(0.f);

	if (GetOwner()->GetName() == L"Player")
	{
		CSceneMgr::GetInst()->GetCurScene()->SetUIText(L"ammoUI", m_iRemainAmmo);
	}
}
/*
void CGun::PlayShotSound(wstring _strSoundName)
{
	CSound* pNewSound = nullptr;
	pNewSound = CResMgr::GetInst()->FindSound(_strSoundName);
	pNewSound->Play();
	pNewSound->SetVolume(10.f);
}*/

void CGun::CreateMissile()
{
	Vec2 vAimDir = GetAimDir();

	//¹Ì»çÀÏ °´Ã¼
	CMissile* pMissile = new CMissile;
	pMissile->SetImage(L"ammo", L"texture\\weapon\\ammo.png");
	pMissile->SetPos(GetPos());
	pMissile->SetScale(Vec2(10.f, 10.f));
	pMissile->SetDir(vAimDir);

	if (GetOwner()->GetName() == L"Player")
		CreateObject(pMissile, GROUP_TYPE::PROJ_PLAYER);
	else
		CreateObject(pMissile, GROUP_TYPE::PROJ_MONSTER);
}