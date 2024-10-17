#include "pch.h"
#include "CTraceState.h"

#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CScene.h"

#include "CMonster.h"

#include "CTimeMgr.h"

CTraceState::CTraceState()
	: CState(MON_STATE::TRACE)
{
}

CTraceState::~CTraceState()
{
}

void CTraceState::update()
{
	// 타겟팅 된 Player를 쫓아간다
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlaterPos = pPlayer->GetPos();
	Vec2 vMonPos = GetMonster()->GetPos();

	// 몬스터 > 플레이어 쪽의 방향 계산
	Vec2 vMonDir = vPlaterPos - vMonPos;
	if (vMonDir.Length() < 20)
		return;

	vMonDir.Normalize();
	
	// 이번 프레임의 이동량을 더해준다
	// 단위벡터에 스칼라 곱 --> 단위벡터의 길이가 스칼라 값만큼 늘어남
	vMonPos += vMonDir * GetMonster()->GetInfo().fSpeed * fDT;
	
	GetMonster()->SetPos(vMonPos);
}

void CTraceState::Enter()
{
}

void CTraceState::Exit()
{
}

