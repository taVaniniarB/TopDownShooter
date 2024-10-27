#include "pch.h"
#include "CIdleState.h"

#include "CSceneMgr.h"
#include "CTimeMgr.h"
#include "CScene.h"

#include "CPlayer.h"
#include "CMonster.h"

CIdleState::CIdleState()
	: CState(MON_STATE::IDLE)
	, m_fCurDetectionDelay(0.f)
	, m_fDetectionDelay(0.3f)
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::update()
{
	// 가만히 있는다

	// 플레이어 위치 받기
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	
	if (!pPlayer)
		return;
	
	Vec2 vPlayerPos = pPlayer->GetPos();

	// 몬스터의 범위 안에 들어오면 추적 상태로 전환
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetPos();

	// 두 위치의 차이값(거리)
	Vec2 vDiff = vPlayerPos - vMonPos;
	float fLen = vDiff.Length();

	// 플레이어가 몬스터 인식 범위 내로 진입 && 중간에 벽이 없을 때
	if (fLen < pMonster->GetInfo().fRecogRange
		&& !CheckWallInRay(vPlayerPos, vMonPos))
	{
		// 지연시간 지난 후 추적 모드로 전환
		if (m_fCurDetectionDelay >= m_fDetectionDelay)
			ChangeAIState(GetAI(), MON_STATE::TRACE);
		// 시간 동기화 필요 > 이벤트 매니저 통해 유예 처리

		m_fCurDetectionDelay += fDT;
	}
}

bool CIdleState::CheckWallInRay(Vec2& vEnd, Vec2& vStart)
{
	// 플레이어 - 몬스터 광선이 벽에 걸리지 않는지 검사
	float dx = vEnd.x - vStart.x;
	float dy = vEnd.y - vStart.y;

	// 기울기가 절대값이 1보다 큰 경우 y를 기준으로 움직이고,
	// 그렇지 않으면 x를 기준으로 움직이는 방식으로 수정
	if (std::abs(dx) > std::abs(dy)) {
		// x 방향으로 움직이며 교차점 구하기
		float xStep = (dx > 0) ? TILE_SIZE : -TILE_SIZE;
		for (float x = std::floor(vStart.x / TILE_SIZE) * TILE_SIZE;
			(dx > 0 && x <= vEnd.x) || (dx < 0 && x >= vEnd.x);
			x += xStep)
		{
			float y = vStart.y + (x - vStart.x) * dy / dx;
			if (isWall((int)(x / TILE_SIZE), (int)(y / TILE_SIZE)))
				return true;
		}
	}
	else {
		// y 방향으로 움직이며 교차점 구하기
		float yStep = (dy > 0) ? TILE_SIZE : -TILE_SIZE;
		for (float y = std::floor(vStart.y / TILE_SIZE) * TILE_SIZE;
			(dy > 0 && y <= vEnd.y) || (dy < 0 && y >= vEnd.y);
			y += yStep)
		{
			float x = vStart.x + (y - vStart.y) * dx / dy;
			if (isWall((int)(x / TILE_SIZE), (int)(y / TILE_SIZE)))
				return true;
		}
	}

	return false;
}

void CIdleState::Enter()
{
}

void CIdleState::Exit()
{
}