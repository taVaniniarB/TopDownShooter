#include "pch.h"
#include "CState.h"

#include "CMonster.h"
#include "AI.h"

#include "CSceneMgr.h"
#include "CScene.h"



CState::CState(MON_STATE _eState)
	: m_eState(_eState)
	, m_pAI(nullptr)
{
}

CState::~CState()
{
}

CMonster* CState::GetMonster()
{
	CMonster* pMonster = m_pAI->GetOwner();
	return pMonster;
}

bool CState::isWall(float x, float y)
{
	if (1 == CSceneMgr::GetInst()->GetCurScene()->GetWallmapNum(x, y))
		return true;

	return false;
}
