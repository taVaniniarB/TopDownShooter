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

			std::reverse(m_vPath.begin(), m_vPath.end());
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
			if (visit[ny][nx] || isWall(nx * TILE_SIZE, ny * TILE_SIZE))
				continue;

			visit[ny][nx] = true;
			Q.push({ nx, ny });
			parent[ny][nx] = cur; // �θ� ��� ���� (��� ������)
		}
	}
}

void CTraceState::update()
{
	Vec2 vEnd = {}; //�̹� ������ ������ ��ǥ
	/*
	// if (��ο� ���� ���� ��)
	if (true)
	{
		// Ÿ���� �� Player�� End ��ǥ�� �����Ͽ� �Ѿư���
		CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
		Vec2 vEnd = pPlayer->GetPos();
	}
	else
	{
		// else (���� ���� ��)
		SearchMovePath();
		vEnd = Vec2(m_vPath[0]) * (TILE_SIZE * 0.5f);
	}
	*/
	SearchMovePath();
	// �� ������ ���Ǵ� path ����
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

	// ���� > �÷��̾� ���� ���� ���
	Vec2 vMonDir = vEnd - vMonPos;
	//if (vMonDir.Length() < 20)
	//	return;

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
