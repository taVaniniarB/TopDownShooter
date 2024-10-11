#include "pch.h"
#include "CGun.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CMissile.h"
#include "CCamera.h"
#include "CResMgr.h"
#include "CSound.h"
#include "CSceneMgr.h"
#include "CScene.h"

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
	float fDelay = GetDelay();
	float fCurDelay = GetCurDelay();
	if (fCurDelay > fDelay)
	{
		if (m_iRemainAmmo > 0)
		{
			CreateMissile();
			// 사운드 재생
			CSound* pNewSound = nullptr;
			switch (m_eGunType)
			{
			case GUN_TYPE::M16:
				pNewSound = CResMgr::GetInst()->FindSound(L"M16");
				break;
			case GUN_TYPE::SHOTGUN:
				pNewSound = CResMgr::GetInst()->FindSound(L"Shotgun");
				break;
			default:
				break;
			}
			pNewSound->Play();
			pNewSound->SetVolume(10.f);

			m_iRemainAmmo--;
			SetCurDelay(0.f);
			
			if (GetOwner()->GetName() == L"Player")
			{
				std::cout << "장탄 수: " << m_iRemainAmmo << "\n";
				CSceneMgr::GetInst()->GetCurScene()->SetUIText(L"ammoUI", m_iRemainAmmo);
			}
		}
	}
	else
	{
		SetCurDelay(fCurDelay + fDT);
	}
}

void CGun::CreateMissile()
{
	Vec2 vMissilePos = GetPos();
	vMissilePos.y -= GetScale().y / 2.f;
	Vec2 vAimDir = GetAimDir();

	//미사일 객체
	CMissile* pMissile = new CMissile;
	pMissile->SetName(L"Missile");
	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(10.f, 10.f));
	pMissile->SetDir(vAimDir);

	CreateObject(pMissile, GROUP_TYPE::PROJ_PLAYER);
}