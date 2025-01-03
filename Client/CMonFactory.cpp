#include "pch.h"
#include "CMonFactory.h"

#include "CMonster.h"
#include "AI.h"

#include "CIdleState.h"
#include "CTraceState.h"

#include "CRigidBody.h"
#include "CResMgr.h"
#include "CWeapon.h"
#include "CWeaponFactory.h"
#include "CHitbox.h"

// 몬스터 팩토리 객체 안 만든다
// 클래스로 선언했지만, 객체 있을 필요 X

CMonster* CMonFactory::CreateMonster(MON_TYPE _eType, Vec2 _vPos, FULL_WEAPON_TYPE _eWeapon)
{
	CMonster* pMon = nullptr;

	switch (_eType)
	{
	case MON_TYPE::NORMAL:
	{
		// 몬스터 객체, AI 객체 생성 > 연결
		// AI 객체에 Normal 타입 몬스터가 가질 수 있는 상태를 추가
		// 몬스터의 현재상태를 IDLE로 지정

		pMon = new CMonster;
		pMon->SetType(_eType);
		pMon->SetPos(_vPos);
		pMon->SetScale(Vec2(64.f, 64.f));
		pMon->SetName(L"Monster");
		pMon->SetTexture(CResMgr::GetInst()->LoadTexture(L"mosnter", L"texture\\ray.bmp"));
		
		

		CHitbox* pNewHitbox = new CHitbox(Vec2(30.f, 30.f));
		pNewHitbox->SetName(L"Hitbox_Monster");
		pMon->SetHitbox(pNewHitbox);

		tMonInfo info = {};
		info.fRecogRange = 300.f;
		info.fSpeed = 100.f;
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
	case MON_TYPE::FRIENDLY:
		pMon = new CMonster;
		pMon->SetType(_eType);
		pMon->SetPos(_vPos);
		pMon->SetScale(Vec2(64.f, 64.f));
		pMon->SetName(L"NPC");
		pMon->SetTexture(CResMgr::GetInst()->LoadTexture(L"mosnter", L"texture\\ray.bmp"));

		break;
	}
	
	if (_eWeapon != FULL_WEAPON_TYPE::END)
	{
		CWeapon* pNewWeapon = CWeaponFactory::CreateWeapon(_eWeapon);
		pMon->SetWeapon(pNewWeapon);
	}
	assert(pMon);

	return pMon;
}
