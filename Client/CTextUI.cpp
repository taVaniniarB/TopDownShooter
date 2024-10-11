#include "pch.h"
#include "CTextUI.h"

#include "CCore.h"

CTextUI::CTextUI()
	: CUI(false)
	, m_eFont(FONT_TYPE::END)
{

}

CTextUI::~CTextUI()
{
}

void CTextUI::render(HDC _dc)
{
	Vec2 vPos = GetPos();

	HFONT hFont = CCore::GetInst()->GetFont(m_eFont);

	HFONT hOldFont = (HFONT)SelectObject(_dc, hFont);
	// l, t, r, b
	RECT rect = { 500, (LONG)vPos.y, (LONG)vPos.x, 100 };
	DrawText(_dc, m_strText.c_str(), -1, &rect, DT_RIGHT | DT_TOP | DT_NOCLIP);

	SelectObject(_dc, hOldFont);
}
