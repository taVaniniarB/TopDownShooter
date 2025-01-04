#include "pch.h"
#include "CTraceState.h"

#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CScene.h"

#include "CMonster.h"

#include "CTimeMgr.h"
#include "CWeapon.h"

struct Node {
	POINT pos; // 현재 노드 좌표
	int g;     // 시작점 ~ 현재 노드 실제 비용
	int h;     // 시작점 ~ 목적지 추정 비용
	int f;     // g + h
	Vec2 parent; // 부모 노드

	bool operator>(const Node& other) const {
		return f > other.f; // 우선순위 큐에서 f 값이 낮은 것이 먼저 나오도록 설정
	}
};


CTraceState::CTraceState()
	: CState(MON_STATE::TRACE)
{
}

CTraceState::~CTraceState()
{
}

void CTraceState::SearchMovePath_AStar(Vec2 vEnd, Vec2 vStart) {
	vEnd *= 1.f / TILE_SIZE;
	vStart *= 1.f / TILE_SIZE;

	m_vPath.clear();

	POINT pEnd = { (int)vEnd.x, (int)vEnd.y };
	POINT pStart = { (int)vStart.x, (int)vStart.y };

	int row = CSceneMgr::GetInst()->GetCurScene()->GetMapRowSize();
	int col = CSceneMgr::GetInst()->GetCurScene()->GetMapColSize();

	// 방문 여부 및 경로 저장용 배열
	std::vector<std::vector<bool>> visit(row, std::vector<bool>(col, false));
	std::vector<std::vector<POINT>> parent(row, std::vector<POINT>(col, { -1, -1 }));

	// A* 방향 이동 배열
	int dx[4] = { 1, 0, -1, 0 };
	int dy[4] = { 0, 1, 0, -1 };

	// 우선순위 큐 (f 값이 낮은 노드를 우선 탐색)
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;

	// 휴리스틱 함수 (맨해튼 거리)
	auto heuristic = [](POINT a, POINT b) {
		return std::abs(a.x - b.x) + std::abs(a.y - b.y);
		};

	// 시작 노드를 큐에 삽입
	Node startNode = { pStart, 0, heuristic(pStart, pEnd), 0 + heuristic(pStart, pEnd), { -1, -1 } };
	openList.push(startNode);
	visit[pStart.y][pStart.x] = true;

	while (!openList.empty()) {
		Node curNode = openList.top();
		openList.pop();
		POINT cur = curNode.pos;

		// 목표 노드에 도달했을 때
		if (cur.x == pEnd.x && cur.y == pEnd.y) {
			POINT p = pEnd;
			while (p.x != -1 && p.y != -1) {
				m_vPath.push_back(p);
				p = parent[p.y][p.x];
			}
			return;
		}

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.x + dx[dir];
			int ny = cur.y + dy[dir];

			// 범위 밖일 경우 무시
			if (nx < 0 || nx >= col || ny < 0 || ny >= row)
				continue;

			// 벽이거나 이미 방문한 경우 무시
			if (isWall(nx, ny) || visit[ny][nx])
				continue;

			// 새 노드의 비용 계산
			int newG = curNode.g + 1; // 인접 노드 이동 비용 (1로 고정)
			int newH = heuristic({ nx, ny }, pEnd);
			int newF = newG + newH;

			// 큐에 추가 및 부모 노드 갱신
			Node nextNode = { { nx, ny }, newG, newH, newF, cur };
			openList.push(nextNode);
			parent[ny][nx] = cur;
			visit[ny][nx] = true;
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

	if (!CSceneMgr::GetInst()->GetCurScene()->GetIsPlayerAlive())
		return;

	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();

	Vec2 vEnd = pPlayer->GetPos();
	Vec2 vStart = GetMonster()->GetPos();

	// 경로에 벽이 있을 때 막힘 방지
	// 충돌체의 네 점에 대하여 점-목적지 직선 경로 충돌검사
	Vec2 vColliderCorrection = { 10.f, 10.f };
	if (isWallInPath(vEnd, vStart + vColliderCorrection)
		|| isWallInPath(vEnd, vStart - vColliderCorrection)
		|| isWallInPath(vEnd, Vec2(vStart.x - vColliderCorrection.x, vStart.y + vColliderCorrection.y))
		|| isWallInPath(vEnd, Vec2(vStart.x + vColliderCorrection.x, vStart.y - vColliderCorrection.y)))
	{
		// 경로 계산
		SearchMovePath_AStar(vEnd, vStart);

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
