#include "pch.h"
#include "CRestartUI.h"
#include "CTitleImg.h"
#include "CResMgr.h"
#include "CCore.h"

CTitleImg::CTitleImg()
{
	Image* pImage = CResMgr::GetInst()->LoadGdiImage(L"TitleBg", L"texture\\UI\\title_bg.png");
	SetBgImage(pImage);

	pImage = CResMgr::GetInst()->LoadGdiImage(L"Title", L"texture\\UI\\title.png");
	SetImage(pImage);

	SetBGOffset({ 0.f, 25.f });

	SetPos(Vec2(CCore::GetInst()->GetWidth() / 2.f, CCore::GetInst()->GetHeight() / 2.f - 60.f));

	SetAmplitude(3.f);
}

CTitleImg::~CTitleImg()
{
}
