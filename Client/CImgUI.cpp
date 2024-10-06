#include "pch.h"
#include "CImgUI.h"
#include "CTexture.h"

CImgUI::CImgUI()
	:CUI(false)
{
}

CImgUI::~CImgUI()
{
}

void CImgUI::render(HDC _dc)
{
	CTexture* pTex = CObject::GetTexture();
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	if (nullptr == pTex)
	{
		return;
	}

	TransparentBlt(_dc
		, (int)(vPos.x)
		, (int)(vPos.y)
		, (int)(vScale.x)
		, (int)(vScale.y)
		, pTex->GetDC()
		, 0, 0, vScale.x, vScale.y, RGB(255, 0, 255));
}
