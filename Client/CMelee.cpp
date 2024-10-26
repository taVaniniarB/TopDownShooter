#include "pch.h"
#include "CMelee.h"
#include "CTimeMgr.h"
#include "CDamageArea.h"

CMelee::CMelee()
{
}

CMelee::~CMelee()
{
}

void CMelee::Attack()
{
}

void CMelee::CreateDamageArea()
{
	Vec2 vAimDir = GetAimDir();
	Vec2 vOffset = GetAimDir().Normalize() * 20.f;
	float attackRange = GetAttackRange();

	//DamageArea °´Ã¼ »ý¼º
	CDamageArea* pDA = new CDamageArea(Vec2(attackRange, attackRange));
	pDA->SetOwner(this);
	
	if (this->GetFullWeaponType() == FULL_WEAPON_TYPE::KNIFE)
		pDA->SetName(L"Knife");
	else
		pDA->SetName(L"Pipe");
	pDA->SetPos(GetPos() + vOffset);
	pDA->SetScale(Vec2(attackRange, attackRange));
	pDA->SetDir(vAimDir);

	if (GetOwner()->GetName() == L"Player")
		CreateObject(pDA, GROUP_TYPE::PROJ_PLAYER);
	else
		CreateObject(pDA, GROUP_TYPE::PROJ_MONSTER);
}