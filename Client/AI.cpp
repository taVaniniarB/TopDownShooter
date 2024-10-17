#include "pch.h"
#include "AI.h"

#include "CState.h"

AI::AI()
	: m_pCurState(nullptr)
	, m_pOwner(nullptr)
{
}

AI::~AI()
{
	// AI 소멸 시, 본인이 가지고 있던 상태 맵의 객체들을 모두 소멸
	Safe_Delete_Map(m_mapState);
}

void AI::update()
{
	// 현재 상태에 맞는 업데이트 코드를 돌린다
	m_pCurState->update();

	
}


void AI::AddState(CState* _pState)
{
	// 동일한 상태가 여러 번 저장되는 경우 방지
	assert(!(GetState(_pState->GetType())));

	// map을 쓴다는 건 순회할 일이 있다는 건가...
	// 배열로 해도 큰 차이 X?
	// State가 많다면 안 쓰는 State 존재할 수 있긴 함

	// 서로 알게 한다
	// AI는 본인의 맵에 오너 몬스터의 모둔 가질 수 있는 상태 객체를 저장
	// 상태 객체는 주인 AI가 누구인지 저장
	m_mapState.insert(make_pair(_pState->GetType(), _pState));
	_pState->m_pAI = this;
}

CState* AI::GetState(MON_STATE _eState)
{
	map<MON_STATE, CState*>::iterator iter = m_mapState.find(_eState);
	if (iter == m_mapState.end())
		return nullptr;

	return iter->second;
}

void AI::SetCurState(MON_STATE _eState)
{
	m_pCurState = GetState(_eState);
	assert(m_pCurState);
}

void AI::ChangeState(MON_STATE _eNextState)
{
	CState* pNextState = GetState(_eNextState);
	
	assert(pNextState != m_pCurState);

	m_pCurState->Exit();

	m_pCurState = GetState(_eNextState);
	
	m_pCurState->Enter();
}

MON_STATE AI::GetCurState()
{
	return m_pCurState->GetType();
}
