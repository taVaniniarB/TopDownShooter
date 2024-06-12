#include "CUI.h"



CUI::CUI()
{
}

CUI::~CUI()
{
}

void CUI::update()
{
}

void CUI::render(HDC _dc)
{// UI는 타일과 마찬가지로 좌상단 기준으로 렌더할 것
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();
	Rectangle(_dc
		, (int)(vPos.x)
		, (int)(vPos.y)
		, (int)(vPos.x + vScale.x)
		, (int)(vPos.y + vScale.y));
}