#include "pch.h"
#include "CAmmoImgUI.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CCore.h"

CAmmoImgUI::CAmmoImgUI()
{
	CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"Ammo", L"texture\\UI\\AmmoUI.bmp");
	SetTexture(pTex);
	SetScale(Vec2(100.f, 100.f));

	Vec2 vResolution = CCore::GetInst()->GetResolution();
	SetPos(Vec2(10.f, vResolution.y - 10.f - GetScale().y));
}

CAmmoImgUI::~CAmmoImgUI()
{
}
