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
			pWeapon->SetDelay(0.2f);
		}
			break;
		case MELEE_TYPE::PIPE:
		{
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
			((CGun*)pWeapon)->SetMaxBullet(26);
			pWeapon->SetDelay(0.5f);

			((CObject*)pWeapon)->SetImgIdx(0);
		}
			break;
		case GUN_TYPE::SHOTGUN:
		{
			((CGun*)pWeapon)->SetMaxBullet(6);
			((CGun*)pWeapon)->SetDelay(1.f);

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
	return nullptr;
}
