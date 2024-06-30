#include "pch.h"
#include "CUIMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CUI.h"
#include "CKeyMgr.h"

CUIMgr::CUIMgr()
	: m_pFocusedUI(nullptr)
{

}

CUIMgr::~CUIMgr()
{

}

void CUIMgr::update()
{
	bool bLbtnTAP = KEY_TAP(KEY::LBTN);
	bool bLbtnAWAY = KEY_AWAY(KEY::LBTN);

	// 1. FocusedUI 확인
	// 혹시나 새로운 UI 포커싱 전환되진 않았는지 확인해서,
	// 현재 기준 최종적인 FocusedUI를 확인해야 함
	m_pFocusedUI = GetFocusedUI();

	// 모든 UI 비활성화 상태이므로 이벤트 처리할 필요도 없으니 리턴
	if (!m_pFocusedUI)
		return;


	// 2. FocusedUI 내에서 부모 UI 포함, 자식 UI 중
	//    실제 타겟팅된 UI 가져와서 이벤트 처리
	CUI* pTargetUI = GetTargetedUI(m_pFocusedUI);

	if (nullptr != pTargetUI)
	{
		pTargetUI->MouseOn();

		if (bLbtnTAP)
		{
			pTargetUI->MouseLbtnDown();
			pTargetUI->m_bLbtnDown = true;
		}
		else if (bLbtnAWAY)
		{
			pTargetUI->MousebtnUp();

			if (pTargetUI->m_bLbtnDown)
			{
				pTargetUI->MouseLbtnClicked();
			}

			pTargetUI->m_bLbtnDown = false;
		}
	}

}

// 씬에서 특정 키를 눌렀을 때 UI를 즉시 소환하고 싶을 때 사용
void CUIMgr::SetFocusedUI(CUI* _pUI)
{
	// 이미 그걸 포커싱 중인 경우, nullptr(포커싱 해제요청)인 경우
	if (m_pFocusedUI == _pUI || nullptr == _pUI)
	{
		m_pFocusedUI = _pUI;
		return;
	}

	m_pFocusedUI = _pUI;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIVector();


	// 반복자용 이터레이터
	vector<CObject*>::iterator iter = vecUI.begin();
	for (; iter != vecUI.end(); ++iter)
	{
		// 벡터 순회로 포커싱 지정할거 찾아내기
		if (m_pFocusedUI == *iter)
		{
			break;
		}
	}

	//순번교체
	vecUI.erase(iter);
	vecUI.push_back(m_pFocusedUI);
}

// 부모 UI들 중에서 Focus 잡은 UI가 누구일까
CUI* CUIMgr::GetFocusedUI()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIVector();

	bool bLbtnTap = KEY_TAP(KEY::LBTN);

	// 기존 포커싱 UI를 받아두고, 변경되었는지 확인
	CUI* pFocusedUI = m_pFocusedUI;


	// 포커싱 넘어간다 = 다른 UI를 눌렀다

	// 이번 프레임에서 좌클릭이 발생하지 않았으면
	// 포커스 전환됐을 가능성도 없으니 리턴
	if (!bLbtnTap)
	{
		return pFocusedUI;
	}

	// 왼쪽 버튼 TAP이 발생했다는 전제

	// 타겟 이터레이터
	vector<CObject*>::iterator targetiter = vecUI.end();
	// 반복자용 이터레이터
	vector<CObject*>::iterator iter = vecUI.begin();
	for (; iter != vecUI.end(); ++iter)
	{
		// isMouseOn: 마우스 올렸는지 아닌지 반환
		// UI별로 각각 본인의 MouseOn 상태를 체크해서 갖고 있음.
		// 하여튼. UI 위에 마우스가 올라와 있는데다가, TAP 발생까지 했다?
		if (((CUI*)*iter)->IsMouseOn())
		{
			targetiter = iter;
		}
		
		// 반복하다보면, 결국은 벡터의 뒤(사용자 기준으로는 가장 앞에 보이는) 애가
		// 포커스의 우선순위를 가진다
	}

	// 이번에 Focus 된 UI가 없다 (허공 클릭) > nullptr 반환
	if (vecUI.end() == targetiter)
	{
		return nullptr;
	}

	pFocusedUI = (CUI*)*targetiter;

	//순번교체(뒤로 옮기기) 위해 erase 후 push back
	vecUI.erase(targetiter);
	vecUI.push_back(pFocusedUI);


	return pFocusedUI;
}

CUI* CUIMgr::GetTargetedUI(CUI* _pParentUI)
{
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	// 1. 부모 UI 포함 모든 자식 검사
	CUI* pTargetUI = nullptr;

	// 함수 호출할 때마다 선언하느니, 정적 변수로 두자
	static list<CUI*> queue;
	static vector<CUI*> vecNoneTarget;

	queue.clear();
	vecNoneTarget.clear();

	queue.push_back(_pParentUI);

	// 큐가 텅 빌 때까지 돌겠다
	while (!queue.empty())
	{
		//헤드 꺼내옴
		CUI* pUI = queue.front();
		queue.pop_front();

		// 큐에서 꺼낸 UI가 Target인지 확인
		// 타겟 UI들 중 우선순위 높은 기준: 낮은 계층 자식 UI
		// 이번 검사 UI가 mouse On 되었을 시 > 일단 타겟으로 지정
		// > 싹 돌리고 나면, 마우스 on이면서 가장 낮은 계층 자식이 타겟으로 되어 있을 것이다
		if (pUI->IsMouseOn())
		{
			// 이번 UI가 마우스on 되면, 이전 반복에서의 타겟 UI는 NonTarget 취급
			if (nullptr != pTargetUI)
			{
				vecNoneTarget.push_back(pTargetUI);
			}
			pTargetUI = pUI;
		}
		else //MouseOn이 아닐 시, 본인이 NoneTarget 
		{
			vecNoneTarget.push_back(pUI);
		}

		// 꺼낸 데이터의 자식 벡터 가져오기
		const vector<CUI*>& vecChild = pUI->GetChildUI();

		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}

	}

	// 왼쪽 버튼 떼면, noneTarget들의 btnDown 체크를 모두 해제
	// 왜 Target은 해제하지 않는가: 클릭 판정 시 방해되기 때문에, nonTarget과 분리함
	// 왜 AWAY 상태일 때만 false로 하는가? 어차피 타깃이 아니면 ㄱㅊ을텐데 
	// >>> UI를 누른 채 화면 밖으로 드래그하는 상황 대비
	if (bLbtnAway)
	{
		for (size_t i = 0; i < vecNoneTarget.size(); ++i)
		{
			vecNoneTarget[i]->m_bLbtnDown = false;
		}
	}

	return pTargetUI;
}