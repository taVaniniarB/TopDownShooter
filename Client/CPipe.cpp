#include "pch.h"
#include "CPipe.h"
#include "CTimeMgr.h"

CPipe::CPipe()
{
}

CPipe::~CPipe()
{
}

void CPipe::Attack()
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
