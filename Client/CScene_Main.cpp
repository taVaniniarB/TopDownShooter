#include "pch.h"
#include "CScene_Main.h"
#include "CKeyMgr.h"
#include "CTitleImg.h"
#include "CCore.h"

CScene_Main::CScene_Main()
{
}

CScene_Main::~CScene_Main()
{
}

void CScene_Main::Enter()
{
	CreateUI();
	// 메인 음악 재생
}

void CScene_Main::Exit()
{
	// 메인 음악 끄기
}

void CScene_Main::update()
{
	CScene::update();
	
	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::STAGE_01);
	}
}

void CScene_Main::render(HDC _dc)
{
	CScene::render(_dc);

	wstring text = L"PRESS ENTER KEY";
	
	Vec2 vPos = Vec2(CCore::GetInst()->GetWidth() / 2.f, CCore::GetInst()->GetHeight() / 2.f + 80.f);

	FONT_TYPE eFont = FONT_TYPE::GULIM;
	HFONT hFont = CCore::GetInst()->GetFont(eFont);

	// 이전 폰트 백업 (기존 폰트를 복원하기 위해)
	HFONT hOldFont = (HFONT)SelectObject(_dc, hFont);

	SetTextColor(_dc, RGB(255, 255, 255));
	SetBkMode(_dc, TRANSPARENT);

	// l, t, r, b
	RECT rect = { LONG(vPos.x - 500), (LONG)vPos.y, (LONG)(vPos.x + 500), (LONG)vPos.y };
	DrawText(_dc, (text ).c_str(), -1, &rect, DT_CENTER | DT_NOCLIP);

	// 폰트를 원래 상태로 복원
	SelectObject(_dc, hOldFont);
}

void CScene_Main::CreateUI()
{
	// 타이틀 이미지
	// Enter to Start
	CObject* pUI = new CTitleImg;
	AddObject(pUI, GROUP_TYPE::UI);
}
