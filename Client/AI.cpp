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
	// AI �Ҹ� ��, ������ ������ �ִ� ���� ���� ��ü���� ��� �Ҹ�
	Safe_Delete_Map(m_mapState);
}

void AI::update()
{
	// ���� ���¿� �´� ������Ʈ �ڵ带 ������
	m_pCurState->update();

	
}


void AI::AddState(CState* _pState)
{
	// ������ ���°� ���� �� ����Ǵ� ��� ����
	assert(!(GetState(_pState->GetType())));

	// map�� ���ٴ� �� ��ȸ�� ���� �ִٴ� �ǰ�...
	// �迭�� �ص� ū ���� X?
	// State�� ���ٸ� �� ���� State ������ �� �ֱ� ��

	// ���� �˰� �Ѵ�
	// AI�� ������ �ʿ� ���� ������ ��� ���� �� �ִ� ���� ��ü�� ����
	// ���� ��ü�� ���� AI�� �������� ����
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
