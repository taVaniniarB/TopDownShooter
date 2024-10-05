#include "pch.h"
#include "CWeaponFactory.h"

#include "CMelee.h"
#include "CGun.h"

#include "CObject.h"
#include "CResMgr.h"

CWeapon* CWeaponFactory::CreateWeapon(WEAPON_TYPE _eWeaponType, MELEE_TYPE _eMeleeType, GUN_TYPE _eGunType)
{
	CWeapon* pWeapon = nullptr;

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
			((CObject*)pWeapon)->SetImgIdx(0);
			pWeapon->SetScale(Vec2(15.f, 7.f));
			pWeapon->SetDelay(0.2f);
		}
			break;
		case MELEE_TYPE::PIPE:
		{
			((CMelee*)pWeapon)->SetType(MELEE_TYPE::KNIFE);
			((CObject*)pWeapon)->SetImgIdx(0);
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
		pWeapon = new CGun;

		CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"gun", L"texture\\weapon\\gun.bmp");
		((CObject*)pWeapon)->SetTexture(pTex);


		switch (_eGunType)
		{
		case GUN_TYPE::M16:
		{
			((CGun*)pWeapon)->SetType(GUN_TYPE::M16);
			((CGun*)pWeapon)->SetMaxMissile(100);
			pWeapon->SetDelay(0.1f);
			pWeapon->SetScale(Vec2(20.f, 10.f));
			((CObject*)pWeapon)->SetImgIdx(0);
		}
			break;
		case GUN_TYPE::SHOTGUN:
		{
			((CGun*)pWeapon)->SetType(GUN_TYPE::SHOTGUN);
			((CGun*)pWeapon)->SetMaxMissile(6);
			pWeapon->SetDelay(0.3f);
			pWeapon->SetScale(Vec2(30.f, 10.f));
			((CObject*)pWeapon)->SetImgIdx(1);
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
	return pWeapon;
}
