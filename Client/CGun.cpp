#include "pch.h"
#include "CGun.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CMissile.h"

CGun::CGun()
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
		if (m_iCurMissile > 0)
		{
			//vPos = GetPos();
			CreateMissile();
			// 사운드 재생

			m_iCurMissile--;
			SetCurDelay(0.f);
		}
	}
	else
	{
		SetCurDelay(fCurDelay + fDT);
	}
}


void CGun::update()
{
	if (KEY_HOLD(KEY::LBTN))
	{
		Attack();
	}
}

void CGun::render()
{
}

void CGun::CreateMissile()
{
	Vec2 vMissilePos = GetFinalPos();
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