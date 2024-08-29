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
	// 네가 알려준 함수를 Call back, 다시 호출 해주겠다
	if (nullptr != m_pFunc)
	{
		m_pFunc(m_param1, m_param2);
	}

	// 객체와 함수가 모두 존재할 경우
	if (m_pSceneInst && m_pSceneFunc)
	{
		// 객체를 통해 멤버함수 포인터 호출하기
		// 포인터 그대로 안 쓰고, (원형.원형)(인자) 혹은 객체 포인터접근
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

