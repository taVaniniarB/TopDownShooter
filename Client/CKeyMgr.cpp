#include "pch.h"
#include "CKeyMgr.h"
#include "CCore.h"

// 키값 enum과 순서 똑같이 매칭한 전역 배열 만듦으로써,
// 인덱스 접근으로 vKey 값을 알게 함
int g_arrVK[(int)KEY::LAST] =
{
	VK_LEFT, //LEFT,
	VK_RIGHT, //RIGHT,
	VK_UP, //UP,
	VK_DOWN, //DOWN,
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'Z',
	'X',
	'C',
	'V',
	'B',



	VK_MENU, // ALT
	VK_CONTROL, // CTRL
	VK_LSHIFT,
	VK_SPACE,
	VK_RETURN, // ENTER
	VK_ESCAPE, // ESC

	VK_LBUTTON,
	VK_RBUTTON,
};


CKeyMgr::CKeyMgr()
{}

CKeyMgr::~CKeyMgr()
{}

void CKeyMgr::init()
{
	// 키 구조체 벡터를 채운다
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}
	//m_vecKey[(int)KEY::LEFT].bPrevPush; // 이런식으로 이전에 눌렸는지와
	//m_vecKey[(int)KEY::LEFT].eState; // 키의 상태 체크 가능하다
}

void CKeyMgr::update()
{
	// 윈도우 포커싱 알아내기
	//HWND hMainWnd = CCore::GetInst()->GetMainHwnd();
	HWND hWnd = GetFocus(); // 현재 포커스 되어 있는 윈도우의 핸들 값을 알려주는 함수
	// 없다면, nullptr 반환

	// 우리 프로그램의 윈도우가 여러 개일 수 있는데,
	// 메인 윈도우만 따질 거라면, hWnd와 hMainWnd가 같은지 다른지만 체크할 수도 있지만
	// 그냥 메인 윈도우 상관 없이 내 프로그램의 다른 윈도우라도 포커스 되어 있을 시 키 입력을 처리하도록 만들자
	if (nullptr != hWnd)
	{
		// 키들의 상태를 변경한다
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// 키가 눌렸다면
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (m_vecKey[i].bPrevPush)
					m_vecKey[i].eState = KEY_STATE::HOLD;
				else
				{
					m_vecKey[i].eState = KEY_STATE::TAP;
					m_vecKey[i].bPrevPush = true;
				}
			}
			else
			{
				if (m_vecKey[i].bPrevPush)
					m_vecKey[i].eState = KEY_STATE::AWAY;
				else
				{
					m_vecKey[i].eState = KEY_STATE::NONE;
				}

				m_vecKey[i].bPrevPush = false;
			}
		}

		// mouse 위치 계산
		POINT ptPos = {};
		
		// GetCursorPos는 전체화면 기준의 커서 좌표 반환(클라이언트 창 기준이 아님)
		GetCursorPos(&ptPos);

		// 클라이언트 윈도우 기준 좌표로 자동으로 계산해주는 함수
		ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptPos);

		m_vCurMousePos = Vec2((float)ptPos.x, (float)ptPos.y);
	}

	//윈도우 포커싱 해제 상태!
	else
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			m_vecKey[i].bPrevPush = false;

			if (KEY_STATE::TAP == m_vecKey[i].eState || KEY_STATE::HOLD == m_vecKey[i].eState)
				m_vecKey[i].eState = KEY_STATE::AWAY;
			else if (KEY_STATE::AWAY == m_vecKey[i].eState)
				m_vecKey[i].eState = KEY_STATE::NONE;
		}
	}
}