#include "pch.h"
#include "CGun.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CMissile.h"
#include "CCamera.h"

CGun::CGun()
	: m_iMaxMissile(0)
	, m_iRemainMissile(0)
	, m_pEmptySound(nullptr)
{
}

CGun::~CGun()
{
}

void CGun::Attack()
{
	float fDelay = GetDelay();
	float fCurDelay = GetCurDelay();
	if (fCurDelay > fDelay)
	{
		if (m_iRemainMissile > 0)
		{
			//vPos = GetPos();
			CreateMissile();
			// ���� ���

			m_iRemainMissile--;
			SetCurDelay(0.f);
			
			std::cout << "��ź ��: " << m_iRemainMissile << "\n";
		}
	}
	else
	{
		SetCurDelay(fCurDelay + fDT);
	}
}


void CGun::update()
{
	CWeapon::update();
	if (GetOwner() && KEY_HOLD(KEY::LBTN))
	{
		Attack();
	}
}


void CGun::CreateMissile()
{
	Vec2 vMissilePos = GetPos();
	vMissilePos.y -= GetScale().y / 2.f;
	Vec2 vAimDir = GetAimDir();

	//�̻��� ��ü
	CMissile* pMissile = new CMissile;
	pMissile->SetName(L"Missile");
	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(10.f, 10.f));
	pMissile->SetDir(vAimDir);

	CreateObject(pMissile, GROUP_TYPE::PROJ_PLAYER);
}