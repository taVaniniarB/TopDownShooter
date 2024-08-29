#include "pch.h"
#include "CMonFactory.h"

#include "CMonster.h"
#include "AI.h"

#include "CIdleState.h"
#include "CTraceState.h"

// ���� ���丮 ��ü �� �����
// Ŭ������ ����������, ��ü ���� �ʿ� X

CMonster* CMonFactory::CreateMonster(MON_TYPE _eType, Vec2 _vPos)
{
	CMonster* pMon = nullptr;

	switch (_eType)
	{
	case MON_TYPE::NORMAL:
	{
		// ���� ��ü, AI ��ü ���� > ����
		// AI ��ü�� Normal Ÿ�� ���Ͱ� ���� �� �ִ� ���¸� �߰�
		// ������ ������¸� IDLE�� ����

		pMon = new CMonster;
		pMon->SetPos(_vPos);

		tMonInfo info = {};
		info.fAttackRAnge = 50.f;
		info.fRecogRange = 300.f;
		info.fSpeed = 50.f;
		info.iHP = 3;

		pMon->SetMonInfo(info);

		AI* pAI = new AI;

		pAI->AddState(new CIdleState);
		pAI->AddState(new CTraceState);
		pAI->SetCurState(MON_STATE::IDLE);

		pMon->SetAI(pAI);
	}
		break;
	case MON_TYPE::RANGE:
		break;
	}
	
	assert(pMon);

	return pMon;
}
