#include "pch.h"
#include "CStage.h"
#include "CWeapon.h"

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

void CStage::SavePlayerWeapon(CWeapon* _pWeapon)
{
	CObject* pNewWeapon = _pWeapon->Clone();
	m_pWeapon = (CWeapon*)pNewWeapon;
}
