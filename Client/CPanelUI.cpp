#include "CPanelUI.h"
#include "CKeyMgr.h"

CPanelUI::CPanelUI()
// CUI의 기본 생성자 없어서 이렇게 초기화 해줌
	: CUI(false)
{
}

CPanelUI::~CPanelUI()
{
}


void CPanelUI::update()
{
}

void CPanelUI::render(HDC _dc)
{
	CUI::render(_dc);
}

void CPanelUI::MouseOn()
{
	// 본인 위에서 LbtnDown 되었으며, on이라면
	// 드래그 중이라는 뜻
	if (IsLbtnDown())
	{
		// 현재 마우스 좌표 - 드래그 시작 시 마우스 좌표
		Vec2 vDiff = MOUSE_POS - m_vDragStart;

		Vec2 vCurPos = GetPos();
		vCurPos += vDiff;
		SetPos(vCurPos);

		m_vDragStart = MOUSE_POS;
	}
}

void CPanelUI::MouseLbtnDown()
{
	CUI::MouseLbtnDown();
	m_vDragStart = MOUSE_POS;
}

void CPanelUI::MousebtnUp()
{
	CUI::MousebtnUp();
}
