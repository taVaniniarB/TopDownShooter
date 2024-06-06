#include "pch.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "CCollider.h"

CMonster::CMonster()
	: m_vCenterPos(Vec2(0.f, 0.f))
	, m_fSpeed(100.f)
	, m_fMaxDistance(50.f)
	, m_iDir(1)
	, m_iHP(5)
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(40.f, 40.f));
}

CMonster::~CMonster()
{
}



void CMonster::update()
{
	Vec2 vCurPos = GetPos();

	//진행 방향에 맞추어 시간 당 seppd 만큼 이동
	vCurPos.x += fDT * m_fSpeed * m_iDir; //iDir이 -1이거나 1이라, 그걸 곱하면 음수 혹은 양수!

	// 배회 거리 기준량을 넘어섰는지 확인
	// DT를 곱하는 방식이라, 쬐까 넘칠 수 있음을 유념
	
	// 양수:초과량 음수:남은이동량
	float fDist = abs(m_vCenterPos.x - vCurPos.x) - m_fMaxDistance;
	
	// 값이 양수 = dt 계산에 의해 최대 거리 초과했다
	if (0.f < fDist)
	{
		// 방향 전환
		m_iDir *= -1;
		vCurPos.x += fDist * m_iDir;
	}

	SetPos(vCurPos);
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	// 충돌한 오브젝트의 정체를 알아내자 (충돌체의 owner 반환하는 GetObj)
	CObject* pOtherObj = _pOther->GetObj();
	// obj 종류 알아내는 방법: 오브젝트에 아이디나 태그명 부여

	if (pOtherObj->GetName() == L"Player_Missile")
	{
		--m_iHP;

		if(m_iHP <= 0)
			DeleteObject(this);
	}

	int a = 0;
}