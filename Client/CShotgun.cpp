#include "pch.h"
#include "CShotgun.h"
#include "CMissile.h"
#include "CTimeMgr.h"
#include "CCamera.h"

CShotgun::CShotgun()
	: m_fRadAngle(7.f)
	, m_iPelletNum(5)
{
}

CShotgun::~CShotgun()
{
}

void CShotgun::Attack()
{
	float fDelay = GetDelay();
	float fCurDelay = GetCurDelay();
	if (fCurDelay >= fDelay)
	{
		if (GetAmmoNum() > 0)
		{
			CreateMissile();
			PlayAttackSound(L"Shotgun");

			// 공통작업 처리
			CGun::Attack();
		}
	}
}

void CShotgun::CreateMissile()
{
	Vec2 vAimDir = GetAimDir();

	float fAngle = m_fRadAngle / m_iPelletNum;

	for (int i = 0; i < m_iPelletNum; ++i)
	{
		//미사일 객체
		CMissile* pMissile = new CMissile;
		pMissile->SetImage(L"ammo", L"texture\\weapon\\ammo.png");
		pMissile->SetPos(GetPos());
		pMissile->SetScale(Vec2(20.f, 20.f));

		// 퍼지는 듯한 총알 구현
		float fAimDir = Vec2ToDgree(vAimDir);

		fAimDir -= m_iPelletNum / 2 * fAngle;
		fAimDir += i * fAngle;

		pMissile->SetDir(DegreeToVec2(fAimDir));

		if (GetOwner()->GetName() == L"Player")
			CreateObject(pMissile, GROUP_TYPE::PROJ_PLAYER);
		else
			CreateObject(pMissile, GROUP_TYPE::PROJ_MONSTER);
	}
}
