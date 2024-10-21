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

void CTraceState::SearchMovePath()
{
	m_vPath.clear();
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vEnd = pPlayer->GetPos() / TILE_SIZE;
	Vec2 vStart = GetMonster()->GetPos() / TILE_SIZE;

	POINT pEnd = { (int)vEnd.x, (int)vEnd.y };
	POINT pStart = { (int)vStart.x, (int)vStart.y };

	int row = CSceneMgr::GetInst()->GetCurScene()->GetMapColSize();
	int col = CSceneMgr::GetInst()->GetCurScene()->GetMapRowSize();

	// 방문 여부 및 경로 저장용 배열
	vector<vector<bool>> visit(row, vector<bool>(col, false));
	vector<vector<POINT>> parent(row, std::vector<POINT>(col, { -1, -1 }));

	int dx[4] = { 1, 0, -1, 0 };
	int dy[4] = { 0, 1, 0, -1 }; // 상하좌우 4방향

	queue<POINT> Q;

	visit[pStart.y][pStart.x] = true;
	Q.push(pStart); // 큐에 0, 0을 삽입

	while (!Q.empty())
	{
		// 큐의 front에 있는 것을 현재노드로 지정하고 pop
		POINT cur = Q.front();
		Q.pop();

		// 목표 노드에 도달했을 때
		if (cur.x == pEnd.x && cur.y == pEnd.y)
		{
			POINT p = pEnd;

			// 부모 노드를 따라 경로를 역추적
			while (p.x != -1 && p.y != -1)
			{
				m_vPath.push_back(p);
				p = parent[p.y][p.x];
			}

			std::reverse(m_vPath.begin(), m_vPath.end());
			return;
		}

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = cur.x + dx[dir];
			int ny = cur.y + dy[dir]; // nx, ny에 dir에서 정한 방향의 인접칸 좌표 들어감

			// 범위 밖일 경우 넘어감
			if (nx < 0 || nx >= col || ny < 0 || ny >= row)
				continue;

			// 이미 방문한 노드 || 1이 아닌 노드인 경우 넘어감
			if (visit[ny][nx] || isWall(nx * TILE_SIZE, ny * TILE_SIZE))
				continue;

			visit[ny][nx] = true;
			Q.push({ nx, ny });
			parent[ny][nx] = cur; // 부모 노드 저장 (경로 추적용)
		}
	}
}

void CTraceState::update()
{
	Vec2 vEnd = {}; //이번 프레임 목적지 좌표
	/*
	// if (경로에 벽이 없을 때)
	if (true)
	{
		// 타겟팅 된 Player를 End 좌표로 지정하여 쫓아간다
		CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
		Vec2 vEnd = pPlayer->GetPos();
	}
	else
	{
		// else (벽이 있을 때)
		SearchMovePath();
		vEnd = Vec2(m_vPath[0]) * (TILE_SIZE * 0.5f);
	}
	*/
	SearchMovePath();
	// 매 프레임 계산되는 path 정보
	if (m_vPath.size() > 1)
	{
		vEnd = (Vec2(m_vPath[1])) * TILE_SIZE;
		vEnd.x += TILE_SIZE / 2;
		vEnd.y += TILE_SIZE / 2;
	}

	Trace(vEnd);
}

void CTraceState::Trace(Vec2 vEnd)
{
	Vec2 vMonPos = GetMonster()->GetPos();

	// 몬스터 > 플레이어 쪽의 방향 계산
	Vec2 vMonDir = vEnd - vMonPos;
	//if (vMonDir.Length() < 20)
	//	return;

	vMonDir.Normalize();

	// 이번 프레임의 이동량을 더해준다
	vMonPos += vMonDir * GetMonster()->GetInfo().fSpeed * fDT;

	GetMonster()->SetPos(vMonPos);
}


void CTraceState::Enter()
{
}

void CTraceState::Exit()
{
}
