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