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
		// �÷��̾� - ���� ������ ���� �ɸ��� �ʴ��� �˻�
		float dx = vPlayerPos.x - vMonPos.x;
		float dy = vPlayerPos.y - vMonPos.y;

		// ���Ⱑ ���밪�� 1���� ū ��� y�� �������� �����̰�,
		// �׷��� ������ x�� �������� �����̴� ������� ����
		if (std::abs(dx) > std::abs(dy)) {
			// x �������� �����̸� ������ ���ϱ�
			float xStep = (dx > 0) ? TILE_SIZE : -TILE_SIZE;
			for (float x = std::floor(vMonPos.x / TILE_SIZE) * TILE_SIZE;
				(dx > 0 && x <= vPlayerPos.x) || (dx < 0 && x >= vPlayerPos.x);
				x += xStep)
			{
				float y = vMonPos.y + (x - vMonPos.x) * dy / dx;
				if (isWall(x, y))
					return;
			}
		}
		else {
			// y �������� �����̸� ������ ���ϱ�
			float yStep = (dy > 0) ? TILE_SIZE : -TILE_SIZE;
			for (float y = std::floor(vMonPos.y / TILE_SIZE) * TILE_SIZE;
				(dy > 0 && y <= vPlayerPos.y) || (dy < 0 && y >= vPlayerPos.y);
				y += yStep)
			{
				float x = vMonPos.x + (y - vMonPos.y) * dx / dy;
				if (isWall(x, y))
					return;
			}
		}

		 
		// State �� ���� ��ȯ
		// �ð� ����ȭ �ʿ� > �̺�Ʈ �Ŵ��� ���� ���� ó��
		// �̺�Ʈ ���
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}
}

void CIdleState::Enter()
{
}

void CIdleState::Exit()
{
}