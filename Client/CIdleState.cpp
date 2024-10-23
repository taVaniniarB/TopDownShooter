#include "pch.h"
#include "CIdleState.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CPlayer.h"
#include "CMonster.h"

CIdleState::CIdleState()
	: CState(MON_STATE::IDLE)
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::update()
{
	// ������ �ִ´�

	// �÷��̾� ��ġ �޾ƿͼ�
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	// ������ ���� �ȿ� ������ ���� ���·� ��ȯ
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetPos();

	// �� ��ġ�� ���̰�(�Ÿ�)
	Vec2 vDiff = vPlayerPos - vMonPos;
	float fLen = vDiff.Length();

	// �÷��̾ ���� �ν� ���� ���� ����
	if (fLen < pMonster->GetInfo().fRecogRange)
	{
		if (CheckWallInRay(vPlayerPos, vMonPos))
			return;
		 
		// State �� ���� ��ȯ
		// �ð� ����ȭ �ʿ� > �̺�Ʈ �Ŵ��� ���� ���� ó��
		// �̺�Ʈ ���
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}
}

bool CIdleState::CheckWallInRay(Vec2& vEnd, Vec2& vStart)
{
	// �÷��̾� - ���� ������ ���� �ɸ��� �ʴ��� �˻�
	float dx = vEnd.x - vStart.x;
	float dy = vEnd.y - vStart.y;

	// ���Ⱑ ���밪�� 1���� ū ��� y�� �������� �����̰�,
	// �׷��� ������ x�� �������� �����̴� ������� ����
	if (std::abs(dx) > std::abs(dy)) {
		// x �������� �����̸� ������ ���ϱ�
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

void CIdleState::Enter()
{
}

void CIdleState::Exit()
{
}