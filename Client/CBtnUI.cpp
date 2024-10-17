#include "CBtnUI.h"

#include "CScene.h"
//#include "CObject.h"


CBtnUI::CBtnUI()
	: CUI(false)
	, m_pFunc(nullptr)
	, m_param1(0)
	, m_param2(0)
	, m_pSceneInst(nullptr)
	, m_pSceneFunc(nullptr)
	, m_pSceneFuncInt(0)
{
}

CBtnUI::~CBtnUI()
{
}

void CBtnUI::MouseOn()
{
}

void CBtnUI::MouseLbtnDown()
{
	CUI::MouseLbtnDown();
}

void CBtnUI::MousebtnUp()
{
	CUI::MousebtnUp();
}

void CBtnUI::MouseLbtnClicked()
{
	// �װ� �˷��� �Լ��� Call back, �ٽ� ȣ�� ���ְڴ�
	if (nullptr != m_pFunc)
	{
		m_pFunc(m_param1, m_param2);
	}

	// ��ü�� �Լ��� ��� ������ ���
	if (m_pSceneInst && m_pSceneFunc)
	{
		// ��ü�� ���� ����Լ� ������ ȣ���ϱ�
		// ������ �״�� �� ����, (����.����)(����) Ȥ�� ��ü ����������
		(m_pSceneInst->*m_pSceneFunc)();
	}

	if (m_pSceneInst && m_pSceneFuncInt)
	{/* ���ڷ� ���� int�� 0�� ���� ����ǵ��� ���� ����
		if (m_param1)
		{
			(m_pSceneInst->*m_pSceneFuncInt)((int)m_param1);
		}*/
		(m_pSceneInst->*m_pSceneFuncInt)((int)m_param1);
	}
}


void CBtnUI::render(HDC _dc)
{
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();
	Rectangle(_dc
		, (int)(vPos.x)
		, (int)(vPos.y)
		, (int)(vPos.x + vScale.x)
		, (int)(vPos.y + vScale.y));

	wchar_t strName[50] = {};

	wcscpy_s(strName, GetName().c_str());

	SetTextColor(_dc, RGB(0, 0, 0));   // �ؽ�Ʈ ���� (����)
	SetBkMode(_dc, TRANSPARENT);       // ��� ���� ����
	TextOut(_dc, (int)(vPos.x + 5), (int)(vPos.y + 5), strName, (int)(wcslen(strName))); // �ؽ�Ʈ ��� ��ġ (�簢���� ���� ���)
}