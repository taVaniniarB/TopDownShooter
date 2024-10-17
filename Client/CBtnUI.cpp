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
	{/* 인자로 들어온 int가 0일 때도 실행되도록 조건 제거
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

	SetTextColor(_dc, RGB(0, 0, 0));   // 텍스트 색상 (검정)
	SetBkMode(_dc, TRANSPARENT);       // 배경 투명 설정
	TextOut(_dc, (int)(vPos.x + 5), (int)(vPos.y + 5), strName, (int)(wcslen(strName))); // 텍스트 출력 위치 (사각형의 좌측 상단)
}