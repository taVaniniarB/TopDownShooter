#include "pch.h"
#include "CWeaponFactory.h"

#include "CMelee.h"
#include "CGun.h"
#include "CM16.h"
#include "CShotgun.h"

#include "CPathMgr.h"
#include "CObject.h"
#include "CResMgr.h"

CWeapon* CWeaponFactory::CreateWeapon(WEAPON_TYPE _eWeaponType, MELEE_TYPE _eMeleeType, GUN_TYPE _eGunType)
{
	CWeapon* pWeapon = nullptr;
	Image* pImage = nullptr;

	switch (_eWeaponType)
	{
	case WEAPON_TYPE::MELEE:
	{
		pWeapon = new CMelee;

		switch (_eMeleeType)
		{
		case MELEE_TYPE::KNIFE:
		{
			((CMelee*)pWeapon)->SetType(MELEE_TYPE::KNIFE);
			//((CObject*)pWeapon)->SetImgIdx(0);
			pWeapon->SetScale(Vec2(15.f, 7.f));
			pWeapon->SetDelay(0.2f);
		}
			break;
		case MELEE_TYPE::PIPE:
		{
			((CMelee*)pWeapon)->SetType(MELEE_TYPE::KNIFE);
			//((CObject*)pWeapon)->SetImgIdx(0);
			pWeapon->SetScale(Vec2(20.f, 7.f));
			pWeapon->SetDelay(0.4f);
		}
			break;
		default:
			break;
		}

	}
		break;
	case WEAPON_TYPE::GUN:
	{
		switch (_eGunType)
		{
		case GUN_TYPE::M16:
		{
			pWeapon = new CM16;
			((CGun*)pWeapon)->SetType(GUN_TYPE::M16);
			((CGun*)pWeapon)->SetMaxMissile(100);
			pWeapon->SetDelay(0.1f);
			pWeapon->SetScale(Vec2(48.f, 14.f));
			pImage = CResMgr::GetInst()->LoadGdiImage(L"M16", L"texture\\weapon\\M16.png");
			pWeapon->SetImage(pImage);
		}
			break;
		case GUN_TYPE::SHOTGUN:
		{
			pWeapon = new CShotgun;
			((CGun*)pWeapon)->SetType(GUN_TYPE::SHOTGUN);
			((CGun*)pWeapon)->SetMaxMissile(6);
			pWeapon->SetDelay(0.3f);
			pWeapon->SetScale(Vec2(50.f, 10.f));
			pImage = CResMgr::GetInst()->LoadGdiImage(L"shotgun", L"texture\\weapon\\shotgun.png");
			pWeapon->SetImage(pImage);
		}
			break;
		default:
			break;
		}

	}
		break;
	default:
		break;
	}


	pWeapon->SetWeaponType(_eWeaponType);
	return pWeapon;
}
