#include "pch.h"
#include "CCombatScene.h"
#include "CTimeMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"

#include "CStage.h"

CCombatScene::CCombatScene()
	: m_iCombo(0)
	, m_iScore(0)
	, m_fCurTime(0)
	, m_fComboTime(0)
{
}

CCombatScene::~CCombatScene()
{
}

void CCombatScene::update()
{// 몬스터 벡터 순회, HP 체크
	const vector<CObject*>& vecMonster = CSceneMgr::GetInst()->GetCurScene()->GetGroupObject(GROUP_TYPE::MONSTER);
	for (size_t i = 0; vecMonster.size() > i; ++i)
	{
		// 몬스터 죽음 > DeleteObject 당하기 전에 실행
		if (vecMonster[i]->IsDead())
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
void CCombatScene::AddScore()
{
	AddCombo();

	// 적 처지 시 점수
	// 기본 100점 + 쌓은 콤보의 50배수만큼 증가한다.
	CStage::GetInst()->AddScore(100 + (m_iCombo * 50));
}

void CCombatScene::AddCombo()
{
	++m_iCombo;
	m_fCurTime = 0;
}

// 모든 몬스터의 정보 관리하고 있기
//	몬스터 벡터 순회하며 HP가 0이 될 때마다 AddCombo와 AddScore 호출
