#include "pch.h"
#include "CUIMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CUI.h"
#include "CKeyMgr.h"

CUIMgr::CUIMgr()
{

}

CUIMgr::~CUIMgr()
{

}

void CUIMgr::update()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	// 모든 최상위 부모 UI들이 들어 있음 (자식 UI는 그 아래 연쇄적으로 달려 있을 것이고)
	const vector<CObject*>& vecUI = pCurScene->GetGroupObject(GROUP_TYPE::UI);



	// 이번 프레임에 딱 눌렸는지 확인
	bool bLbtnTap = KEY_TAP(KEY::LBTN);
	// 딱 떼졌는지 확인
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	// 둘 다 false일 수는 있어도 둘 다 true일 수는 없음 > else if 사용

	for (size_t i = 0; i < vecUI.size(); ++i)
	{
		// obejct가 아닌 ui에만 있는 함수 사용 위해 다운캐스팅
		CUI* pUI = (CUI*)vecUI[i];

		pUI = GetTargetedUI(pUI);

		// 타겟 UI가 null이 아니다 = 마우스 올린 UI 존재한다
		// = 그 UI의 MouseOn 호출
		if (nullptr != pUI)
		{
			pUI->MouseOn();

			// 탭되었다
			if (bLbtnTap)
			{
				pUI->MouseLbtnDown();
				pUI->m_bLbtnDown = true; // 눌렸음을 저장
			}
			else if (bLbtnAway)
			{
				pUI->MousebtnUp();

				if (pUI->m_bLbtnDown)
				{
					pUI->MouseLbtnClicked();
				}

				pUI->m_bLbtnDown = false;
			}
			//클릭이란?
			// LbtnDown 다음 LbtnUp 발생
		}
	}
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
		else //그렇지 않을 시
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

	// 왼쪽 버튼 떼면, 눌렸던 체크를 다시 해제
	if (bLbtnAway)
	{
		for (size_t i = 0; i < vecNoneTarget.size(); ++i)
		{
			vecNoneTarget[i]->m_bLbtnDown = false;
		}
	}

	return pTargetUI;
}
