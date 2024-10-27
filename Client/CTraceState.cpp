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

	// �湮 ���� �� ��� ����� �迭
	vector<vector<bool>> visit(row, vector<bool>(col, false));
	vector<vector<POINT>> parent(row, std::vector<POINT>(col, { -1, -1 }));

	int dx[4] = { 1, 0, -1, 0 };
	int dy[4] = { 0, 1, 0, -1 }; // �����¿� 4����

	queue<POINT> Q;

	visit[pStart.y][pStart.x] = true;
	Q.push(pStart); // ť�� 0, 0�� ����

	while (!Q.empty())
	{
		// ť�� front�� �ִ� ���� ������� �����ϰ� pop
		POINT cur = Q.front();
		Q.pop();

		// ��ǥ ��忡 �������� ��
		if (cur.x == pEnd.x && cur.y == pEnd.y)
		{
			POINT p = pEnd;

			// �θ� ��带 ���� ��θ� ������
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
			int ny = cur.y + dy[dir]; // nx, ny�� dir���� ���� ������ ����ĭ ��ǥ ��

			// ���� ���� ��� �Ѿ
			if (nx < 0 || nx >= col || ny < 0 || ny >= row)
				continue;

			// �̹� �湮�� ��� || 1�� �ƴ� ����� ��� �Ѿ
			if (visit[ny][nx] || isWall(nx, ny))
				continue;

			visit[ny][nx] = true;
			Q.push({ nx, ny });
			parent[ny][nx] = cur; // �θ� ��� ���� (��� ������)
		}
	}
}

bool CTraceState::isWallInPath(Vec2 vEnd, Vec2 vStart)
{
	// ���� ����
	float dx = vEnd.x - vStart.x;
	float dy = vEnd.y - vStart.y;

	if (std::abs(dx) > std::abs(dy)) {
		// x �������� �����̸� ������ ���ϱ�
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
		// y �������� �����̸� ������ ���ϱ�
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
	//�̹� ������ ������ ��ǥ
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();

	if (!pPlayer)
		return;

	Vec2 vEnd = pPlayer->GetPos();
	Vec2 vStart = GetMonster()->GetPos();

	// ��ο� ���� ���� ��
	// �浹ü�� �� ���� ���Ͽ� ��� �˻��ؾ� �ұ�?
	// Ȥ�� ���⿡ ���� �浹ü �� �ϳ��� �߰��� �˻�
	// �� ��� Ÿ�ϻ���� ������ ���� ��ǥ�� �״�θ� ����ؾ� ��
	Vec2 vColliderCorrection = { 10.f, 10.f };
	if (isWallInPath(vEnd, vStart + vColliderCorrection)
		|| isWallInPath(vEnd, vStart - vColliderCorrection)
		|| isWallInPath(vEnd, Vec2(vStart.x - vColliderCorrection.x, vStart.y + vColliderCorrection.y))
		|| isWallInPath(vEnd, Vec2(vStart.x + vColliderCorrection.x, vStart.y - vColliderCorrection.y)))
	{
		// �� ������ ���Ǵ� path ����
		SearchMovePath(vEnd, vStart);

		if (m_vPath.size() > 1)
		{
			vEnd = (Vec2(m_vPath[m_vPath.size() - 2])) * TILE_SIZE;
			vEnd.x += TILE_SIZE / 2;
			vEnd.y += TILE_SIZE / 2;
		}
		else
		{
			// ���� ��� ������ ���� ��
			// �� ���ڸ��ų�, �� �� �ִ� ��ΰ� �������� �ʴ� ���
			return;
		}
	}

	Vec2 vPlayerPos = pPlayer->GetPos();
	float range = GetMonster()->GetWeapon()->GetAttackRange();
	// ��ο� ���� ���ٸ� �����Ѵ�
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

	// ���� > �÷��̾� ���� ���� ���
	Vec2 vMonDir = vEnd - vMonPos;

	Vec2 vPlayerToMonster = vPlayerPos - vMonPos;
	if (vPlayerToMonster.Length() < 20)
		return;

	vMonDir.Normalize();

	// �̹� �������� �̵����� �����ش�
	vMonPos += vMonDir * GetMonster()->GetInfo().fSpeed * fDT;

	GetMonster()->SetPos(vMonPos);
}


void CTraceState::Enter()
{
}

void CTraceState::Exit()
{
}
