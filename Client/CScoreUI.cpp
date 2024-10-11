#include "CScoreUI.h"
#include "CCore.h"
#include "CStage.h"

CScoreUI::CScoreUI()
	: m_iPrevScore(0)
{
	float pdding = 20;
	POINT res = CCore::GetInst()->GetResolution();
	SetPos(Vec2(res.x - pdding, pdding));
	SetText(L"0");

	SetFont(FONT_TYPE::SYSTEM);
}

CScoreUI::~CScoreUI()
{
}

void CScoreUI::update()
{
	const int& score = CStage::GetInst()->GetScore();

	if (score == m_iPrevScore)
	{
		return;
	}

	wstring scoreText = std::to_wstring(score);
	SetText(scoreText);
	m_iPrevScore = score;
}

void CScoreUI::render(HDC _dc)
{
	wstring text = GetText();
	Vec2 vPos = GetPos();
	FONT_TYPE eFont = GetFont();

	HFONT hFont = CCore::GetInst()->GetFont(eFont);

	// 이전 폰트 백업 (기존 폰트를 복원하기 위해)
	HFONT hOldFont = (HFONT)SelectObject(_dc, hFont);
	// l, t, r, b
	RECT rect = { 500, (LONG)vPos.y, (LONG)vPos.x, 100 };
	DrawText(_dc, text.c_str(), -1, &rect, DT_RIGHT | DT_TOP | DT_NOCLIP);

	// 폰트를 원래 상태로 복원
	SelectObject(_dc, hOldFont);
}
