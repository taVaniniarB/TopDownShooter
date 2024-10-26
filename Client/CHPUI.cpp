#include "pch.h"
#include "CHPUI.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CCore.h"

CHPUI::CHPUI()
	: m_iHP(0)
	, m_iDist(30.f)
{
	//SetVisable(true);
	CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"Heart", L"texture\\UI\\heart.bmp");
	SetTexture(pTex);
	SetScale(Vec2(32.f, 32.f));

	Vec2 vResolution = CCore::GetInst()->GetResolution();
	SetPos(Vec2(vResolution.x / 2.f, 10.f));
}

CHPUI::~CHPUI()
{
}

void CHPUI::render(HDC _dc)
{
	if (!GetIsVisable()) return;

	CTexture* pTex = CObject::GetTexture();
	if (nullptr == pTex)
		return;

	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	for (int i = 0; i < m_iHP; ++i)
	{
		vPos.x += m_iDist;

		TransparentBlt(_dc
			, (int)(vPos.x)
			, (int)(vPos.y)
			, (int)(vScale.x)
			, (int)(vScale.y)
			, pTex->GetDC()
			, 0, 0, (int)vScale.x, (int)vScale.y, RGB(255, 0, 255));
	}
}
