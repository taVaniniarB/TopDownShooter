#include "pch.h"
#include "CMonFactory.h"

#include "CMonster.h"
#include "AI.h"

#include "CIdleState.h"
#include "CTraceState.h"

#include "CRigidBody.h"
#include "CResMgr.h"

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
		pMon->SetType(_eType);
		pMon->SetPos(_vPos);
		pMon->SetScale(Vec2(64.f, 64.f));
		pMon->SetName(L"Monster");
		pMon->SetTexture(CResMgr::GetInst()->LoadTexture(L"mosnter", L"texture\\ray.bmp"));
		
		tMonInfo info = {};
		info.fRecogRange = 300.f;
		info.fSpeed = 50.f;
		info.iHP = 1;

		pMon->SetMonInfo(info);

		pMon->CreateRigidBody();
		pMon->GetRigidBody()->SetMass(1.f);

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
