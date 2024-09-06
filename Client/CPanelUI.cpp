#include "CPanelUI.h"
#include "CKeyMgr.h"

CPanelUI::CPanelUI()
// CUI�� �⺻ ������ ��� �̷��� �ʱ�ȭ ����
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
	// ���� ������ LbtnDown �Ǿ�����, on�̶��
	// �巡�� ���̶�� ��
	if (IsLbtnDown())
	{
		// ���� ���콺 ��ǥ - �巡�� ���� �� ���콺 ��ǥ
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
