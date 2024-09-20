#include "pch.h"
#include "CMousePtr.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CKeyMgr.h"


CMousePtr::CMousePtr()
{
	ShowCursor(FALSE);
	SetTexture(CResMgr::GetInst()->LoadTexture(L"cursor", L"texture\\scope.bmp"));
	SetScale(Vec2(42.f, 42.f));
}

CMousePtr::~CMousePtr()
{
	ShowCursor(TRUE);
}

void CMousePtr::update()
{
	SetPos(MOUSE_POS);
}

void CMousePtr::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();
	CTexture* pTex = GetTexture();

	TransparentBlt(_dc
		, (int)(vPos.x - vScale.x / 2.f)
		, (int)(vPos.y - vScale.y / 2.f)
		, (int)(vScale.x)
		, (int)(vScale.y)
		, pTex->GetDC()
		, (int)(0) // 이미지 상 좌상단좌표 x,y
		, (int)(0)
		, (int)(vScale.x)
		, (int)(vScale.y)
		, RGB(255, 0, 255));
}