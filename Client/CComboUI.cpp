#include "pch.h"
#include "CComboUI.h"
#include "CCore.h"

CComboUI::CComboUI()
{
	float pdding = 20.f;
	POINT res = CCore::GetInst()->GetResolution();
	SetPos(Vec2(pdding, pdding));
	SetText(L"0");

	SetFont(FONT_TYPE::SYSTEM);
}

CComboUI::~CComboUI()
{
}

void CComboUI::render(HDC _dc)
{
	if (GetIsVisable())
	{
		wstring text = GetText();
		Vec2 vPos = GetPos();
		FONT_TYPE eFont = GetFont();

		HFONT hFont = CCore::GetInst()->GetFont(eFont);

		SetTextColor(_dc, RGB(135, 0, 18));
		SetBkMode(_dc, TRANSPARENT);

		// 이전 폰트 백업 (기존 폰트를 복원하기 위해)
		HFONT hOldFont = (HFONT)SelectObject(_dc, hFont);
		// l, t, r, b
		RECT rect = { (LONG)vPos.x, (LONG)vPos.y, 100, 100 };
		DrawText(_dc, (text + L"x").c_str(), -1, &rect, DT_LEFT | DT_TOP | DT_NOCLIP);

		// 폰트를 원래 상태로 복원
		SelectObject(_dc, hOldFont);
	}
}