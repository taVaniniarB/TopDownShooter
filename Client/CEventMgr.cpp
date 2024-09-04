#include "pch.h"
#include "CEventMgr.h"
#include "CObject.h"
#include "CScene.h"
#include "CSceneMgr.h"
#include "CUIMgr.h"
#include "AI.h"
#include "CState.h"

CEventMgr::CEventMgr()
{
}

CEventMgr::~CEventMgr()
{
}

void CEventMgr::update()
{
	//============================================
	// 이전 프레임에서 등록해둔 DeadObject들 삭제
	//============================================
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		delete m_vecDead[i];
	}
	m_vecDead.clear();


	//==========
	//Event 처리
	//==========
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		Excute(m_vecEvent[i]);

		// Scene Change 발생 시, 다음의 이벤트들은 무시한다.
		if (m_vecEvent[i].eEven == EVENT_TYPE::SCENE_CHANGE)
			break;
	}
	//일괄처리 후 벡터 클리어
	m_vecEvent.clear();
}

void CEventMgr::Excute(const tEvent& _eve)
{
	switch (_eve.eEven)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		// lParam: Object 주소
		// wParam: Object Group type

		CObject* pNewObj = (CObject*)_eve.lParam;
		GROUP_TYPE eType = (GROUP_TYPE)_eve.wParam;
		
		CSceneMgr::GetInst()->GetCurScene()->AddObject(pNewObj, eType);
	}
		break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam: Object 주소
		// Object를 Dead 상태로 변경
		// 삭제예정 오브젝트들을 모아둔다
		CObject* pDeadObj = (CObject*)_eve.lParam;
		pDeadObj->SetDead();
		m_vecDead.push_back(pDeadObj);
	}

		break;
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam: Next Scene Type

		// Scene 변경
		CSceneMgr::GetInst()->ChangeScene((SCENE_TYPE)_eve.lParam);
		// 포커스 된 UI 해제 (이전 Scene의 UI를 가리키고 있기 때문)
		CUIMgr::GetInst()->SetFocusedUI(nullptr);
	}
	break;
	case EVENT_TYPE::CHANGE_AI_STATE:
	{
		// lParam: AI
		// wParam: Next State Type

		AI* pAI = (AI*)_eve.lParam;
		MON_STATE eNextState = (MON_STATE)_eve.wParam;

		pAI->SetCurState(eNextState);
	}

		break;
	}
}
