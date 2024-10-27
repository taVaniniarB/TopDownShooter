#include "pch.h"
#include "CTraceState.h"

#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CScene.h"

#include "CMonster.h"

#include "CTimeMgr.h"
#include "CWeapon.h"
//#include "CCollider.h"

CTraceState::CTraceState()
	: CState(MON_STATE::TRACE)
{
}

CTraceState::~CTraceState()
{
}

void CTraceState::SearchMovePath(Vec2 vEnd, Vec2 vStart)
{
	vEnd *= 1.f / TILE_SIZE;
	vStart *= 1.f / TILE_SIZE;

	m_vPath.clear();

	POINT pEnd = { (int)vEnd.x, (int)vEnd.y };
	POINT pStart = { (int)vStart.x, (int)vStart.y };

	int row = CSceneMgr::GetInst()->GetCurScene()->GetMapRowSize();
	int col = CSceneMgr::GetInst()->GetCurScene()->GetMapColSize();

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
			if (visit[ny][nx] || isWall(nx, ny))
				continue;

			visit[ny][nx] = true;
			Q.push({ nx, ny });
			parent[ny][nx] = cur; // 부모 노드 저장 (경로 추적용)
		}
	}
}

bool CTraceState::isWallInPath(Vec2 vEnd, Vec2 vStart)
{
	// 방향 벡터
	float dx = vEnd.x - vStart.x;
	float dy = vEnd.y - vStart.y;

	if (std::abs(dx) > std::abs(dy)) {
		// x 방향으로 움직이며 교차점 구하기
		float xStep = (dx > 0) ? TILE_SIZE : -TILE_SIZE;
		for (float x = std::floor(vStart.x / TILE_SIZE) * TILE_SIZE;
			(dx > 0 && x <= vEnd.x) || (dx < 0 && x >= vEnd.x);
			x += xStep)
		{
			float y = vStart.y + (x - vStart.x) * dy / dx;
			if (isWall(x / TILE_SIZE, y / TILE_SIZE))
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

void CTraceState::update()
{
	//이번 프레임 목적지 좌표
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();

	if (!pPlayer)
		return;

	Vec2 vEnd = pPlayer->GetPos();
	Vec2 vStart = GetMonster()->GetPos();

	// 경로에 벽이 있을 때
	// 충돌체의 네 점에 대하여 모두 검사해야 할까?
	// 혹은 기울기에 따라 충돌체 점 하나를 추가로 검사
	// 이 경우 타일사이즈를 나누지 않은 좌표값 그대로를 사용해야 함
	Vec2 vColliderCorrection = { 10.f, 10.f };
	if (isWallInPath(vEnd, vStart + vColliderCorrection)
		|| isWallInPath(vEnd, vStart - vColliderCorrection)
		|| isWallInPath(vEnd, Vec2(vStart.x - vColliderCorrection.x, vStart.y + vColliderCorrection.y))
		|| isWallInPath(vEnd, Vec2(vStart.x + vColliderCorrection.x, vStart.y - vColliderCorrection.y)))
	{
		// 매 프레임 계산되는 path 정보
		SearchMovePath(vEnd, vStart);

		if (m_vPath.size() > 1)
		{
			vEnd = (Vec2(m_vPath[m_vPath.size() - 2])) * TILE_SIZE;
			vEnd.x += TILE_SIZE / 2;
			vEnd.y += TILE_SIZE / 2;
		}
		else
		{
			// 다음 경로 정보가 없을 때
			// 즉 제자리거나, 갈 수 있는 경로가 존재하지 않는 경우
			return;
		}
	}

	Vec2 vPlayerPos = pPlayer->GetPos();
	float range = GetMonster()->GetWeapon()->GetAttackRange();
	// 경로에 벽이 없다면 공격한다
	if (!isWallInPath(vPlayerPos, vStart)
		&& (vPlayerPos - vStart).Length() <= range)
		Attack();

	Trace(vEnd);
}

void CTraceState::Attack()
{
	Vec2 vPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayer()->GetPos();
	CWeapon* pMonWeapon = GetMonster()->GetWeapon();
	if (pMonWeapon)
	{
		Vec2 vAimDir = vPlayerPos - pMonWeapon->GetPos();
		pMonWeapon->SetAimDir(vAimDir);
		pMonWeapon->Attack();
	}
}

void CTraceState::Trace(Vec2 vEnd)
{
	Vec2 vMonPos = GetMonster()->GetPos();
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	// 몬스터 > 플레이어 쪽의 방향 계산
	Vec2 vMonDir = vEnd - vMonPos;

	Vec2 vPlayerToMonster = vPlayerPos - vMonPos;
	if (vPlayerToMonster.Length() < 20)
		return;

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
