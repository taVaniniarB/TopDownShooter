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
	
}

void CBtnUI::MousebtnUp()
{
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
	{
		if (m_param1)
		{
			(m_pSceneInst->*m_pSceneFuncInt)((int)m_param1);
		}
	}
}

