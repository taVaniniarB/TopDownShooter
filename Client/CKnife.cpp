#include "pch.h"
#include "CKnife.h"
#include "CTimeMgr.h"

CKnife::CKnife()
{
}

CKnife::~CKnife()
{
}

void CKnife::Attack()
{
	float fDelay = GetDelay();
	float fCurDelay = GetCurDelay();
	if (fCurDelay > fDelay)
	{
		CreateDamageArea();
		PlayAttackSound(L"Swing1");

		SetCurDelay(0.f);
	}
	else
	{
		SetCurDelay(fCurDelay + fDT);
	}
}
