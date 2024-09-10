#include "pch.h"
#include "CGun.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"

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
		if (m_iCurBullet > 0)
		{
			//vPos = GetPos();
			CreateMissile();
			// 사운드 재생

			m_iCurBullet--;
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
	Vec2 vPos = GetPos();
}