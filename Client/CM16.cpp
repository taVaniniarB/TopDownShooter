#include "pch.h"
#include "CM16.h"
#include "CMissile.h"
#include "CTimeMgr.h"
#include "CCamera.h"

CM16::CM16()
{
}

CM16::~CM16()
{
}

void CM16::Attack()
{
	float fDelay = GetDelay();
	float fCurDelay = GetCurDelay();
	if (fCurDelay >= fDelay)
	{
		if (GetAmmoNum() > 0)
		{
			CreateMissile();
			PlayAttackSound(L"M16");

			// 공통작업 처리
			CGun::Attack();
		}
	}
}

void CM16::CreateMissile()
{
	CGun::CreateMissile();
}