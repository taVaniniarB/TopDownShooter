#include "pch.h"
#include "CWeapon.h"
#include "CStage.h"

CStage::CStage()
	: m_iScore(0)
	, m_pWeapon(nullptr)
	, m_vScene{}
{
}

CStage::~CStage()
{
	delete(m_pWeapon);
}

void CStage::AddScore(int _iScore)
{
	m_iScore += _iScore;
}

void CStage::SavePlayerWeapon(CWeapon* _pWeapon)
{
	if (_pWeapon == nullptr)
	{
		return;
	}

	CObject* pNewWeapon = _pWeapon->Clone();
	m_pWeapon = (CWeapon*)pNewWeapon;
}

