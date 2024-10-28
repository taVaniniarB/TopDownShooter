#include "pch.h"
#include "CAmmoUI.h"
#include "CCore.h"
#include "SelectGDI.h"

CAmmoUI::CAmmoUI()
{
	SetVisable(false);
	int padding = 25;
	SetPos(Vec2(80, CCore::GetInst()->GetHeight() - padding));
	SetText(L"0");
	SetFont(FONT_TYPE::SYSTEM);
}

CAmmoUI::~CAmmoUI()
{
}

void CAmmoUI::render(HDC _dc)
{
	if (GetIsVisable())
	{
		wstring text = GetText();
		Vec2 vPos = GetPos();
		FONT_TYPE eFont = GetFont();

		HFONT hFont = CCore::GetInst()->GetFont(eFont);

		SetTextColor(_dc, RGB(255, 255, 255));
		SetBkMode(_dc, TRANSPARENT);

		// ���� ��Ʈ ��� (���� ��Ʈ�� �����ϱ� ����)
		HFONT hOldFont = (HFONT)SelectObject(_dc, hFont);
		// l, t, r, b
		RECT rect = { (LONG)vPos.x, 100, 1000, (LONG)vPos.y};
		DrawText(_dc, text.c_str(), -1, &rect, DT_BOTTOM | DT_LEFT | DT_NOCLIP | DT_SINGLELINE);
		// ��Ʈ�� ���� ���·� ����
		SelectObject(_dc, hOldFont);

		//PEN_TYPE ePen = PEN_TYPE::GREEN;
		//SelectGDI p(_dc, ePen);
		//SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);
		//Rectangle(_dc, vPos.x, 100, 1000, (LONG)vPos.y);
	}
}
