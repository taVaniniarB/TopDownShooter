#include "pch.h"
#include "CWeaponFactory.h"

#include "CMelee.h"
#include "CKnife.h"
#include "CPipe.h"
#include "CGun.h"
#include "CM16.h"
#include "CShotgun.h"

#include "CPathMgr.h"
#include "CObject.h"
#include "CResMgr.h"


CWeapon* CWeaponFactory::CreateWeapon(FULL_WEAPON_TYPE _eFullWeaponType)
{
	CWeapon* pWeapon = nullptr;
	Image* pImage = nullptr;
	WEAPON_TYPE eWeaponType = WEAPON_TYPE::MELEE;


	switch (_eFullWeaponType)
	{
	case FULL_WEAPON_TYPE::M16:
	{
		eWeaponType = WEAPON_TYPE::GUN;
		pWeapon = new CM16;
		((CGun*)pWeapon)->SetType(GUN_TYPE::M16);
		((CGun*)pWeapon)->SetMaxMissile(24);
		pWeapon->SetDelay(0.1f);
		pWeapon->SetScale(Vec2(48.f, 14.f));
		pWeapon->SetAttackRange(INFINITE);
		pImage = CResMgr::GetInst()->LoadGdiImage(L"M16", L"texture\\weapon\\M16.png");
		pWeapon->SetImage(pImage);
	}
		break;
	case FULL_WEAPON_TYPE::SHOTGUN:
	{
		eWeaponType = WEAPON_TYPE::GUN;
		pWeapon = new CShotgun;
		((CGun*)pWeapon)->SetType(GUN_TYPE::SHOTGUN);
		((CGun*)pWeapon)->SetMaxMissile(6);
		pWeapon->SetDelay(0.4f);
		pWeapon->SetScale(Vec2(50.f, 10.f));
		pWeapon->SetAttackRange(INFINITE);
		pImage = CResMgr::GetInst()->LoadGdiImage(L"shotgun", L"texture\\weapon\\shotgun.png");
		pWeapon->SetImage(pImage);
	}
		break;
	case FULL_WEAPON_TYPE::KNIFE:
	{
		eWeaponType = WEAPON_TYPE::MELEE;
		pWeapon = new CKnife;
		((CMelee*)pWeapon)->SetType(MELEE_TYPE::KNIFE);
		pWeapon->SetScale(Vec2(20.f, 10.f));
		pWeapon->SetDelay(0.15f);

		pWeapon->SetAttackRange(25.f);

		pImage = CResMgr::GetInst()->LoadGdiImage(L"Knife", L"texture\\weapon\\knife.png");
		pWeapon->SetImage(pImage);
	}
		break;
	case FULL_WEAPON_TYPE::PIPE:
	{
		eWeaponType = WEAPON_TYPE::MELEE;
		pWeapon = new CPipe;
		((CMelee*)pWeapon)->SetType(MELEE_TYPE::PIPE);
		pWeapon->SetScale(Vec2(40.f, 15.f));
		pWeapon->SetDelay(0.25f);

		pWeapon->SetAttackRange(40.f);

		pImage = CResMgr::GetInst()->LoadGdiImage(L"Pipe", L"texture\\weapon\\pipe.png");
		pWeapon->SetImage(pImage);
	}
		break;
	default:
		break;
	}

	pWeapon->SetFullWeaponType(_eFullWeaponType);
	pWeapon->SetWeaponType(eWeaponType);
	return pWeapon;
}
