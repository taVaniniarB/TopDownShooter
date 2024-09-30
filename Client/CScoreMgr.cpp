#include "pch.h"
#include "CScoreMgr.h"
#include "CTimeMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"

CScoreMgr::CScoreMgr()
	: m_iCombo(0)
	, m_iScore(0)
	, m_fCurTime(0)
	, m_fComboTime(0)
{
	// 적 처치로부터 1초 내로 적을 처치해야 콤보가 증가한다.
	// 처치하지 못하면 콤보가 리셋된다.
	m_fComboTime = 1.f;
}

CScoreMgr::~CScoreMgr()
{
}

void CScoreMgr::update()
{
	// 몬스터 벡터 순회, HP 체크
	const vector<CObject*>& vecMonster = CSceneMgr::GetInst()->GetCurScene()->GetGroupObject(GROUP_TYPE::MONSTER);
	for (size_t i = 0; vecMonster.size() > i; ++i)
	{
		// 몬스터 죽음 > DeleteObject 당하기 전에 실행
		if (0 == vecMonster[i]->IsDead())
		{
			AddScore();
		}
	}
	
	m_fCurTime += fDT;

	if (m_fCurTime > m_fComboTime)
	{
		ResetCombo();
	}
}

// 몬스터의 HP가 0이 됐을 때 호출
void CScoreMgr::AddScore()
{
	AddCombo();

	// 적 처지 시 점수
	// 기본 100점 + 쌓은 콤보의 50배수만큼 증가한다.
	m_iScore += 100 + (m_iCombo * 50);

	// 그럼 시간 상
	// 적을 죽이면 AddCombo 먼저 실행돼야 함.
}

void CScoreMgr::AddCombo()
{
	++m_iCombo;
	m_fCurTime = 0;
}

// 모든 몬스터의 정보 관리하고 있기
//	몬스터 벡터 순회하며 HP가 0이 될 때마다 AddCombo와 AddScore 호출
// or
// 모든 몬스터의 Update에서 HP가 0이 되면 호출해주기
