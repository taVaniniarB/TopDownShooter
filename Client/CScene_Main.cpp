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
	// ���� ���� ���
}

void CScene_Main::Exit()
{
	// ���� ���� ����
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

	// ���� ��Ʈ ��� (���� ��Ʈ�� �����ϱ� ����)
	HFONT hOldFont = (HFONT)SelectObject(_dc, hFont);

	SetTextColor(_dc, RGB(255, 255, 255));
	SetBkMode(_dc, TRANSPARENT);

	// l, t, r, b
	RECT rect = { LONG(vPos.x - 500), (LONG)vPos.y, (LONG)(vPos.x + 500), (LONG)vPos.y };
	DrawText(_dc, (text ).c_str(), -1, &rect, DT_CENTER | DT_NOCLIP);

	// ��Ʈ�� ���� ���·� ����
	SelectObject(_dc, hOldFont);
}

void CScene_Main::CreateUI()
{
	// Ÿ��Ʋ �̹���
	// Enter to Start
	CObject* pUI = new CTitleImg;
	AddObject(pUI, GROUP_TYPE::UI);
}
