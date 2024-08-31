#include "pch.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CObject.h"


CRigidBody::CRigidBody()
	: m_pOwner(nullptr)
	, m_fMass(1.f)
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::finalUpdate()
{
	// finalupdate 시점에서 m_vForce는
	// 그 프레임에서의 최종 방향과 크기 정보가 담겨 있는 상태
	// 가속도를 구해보자.
	
	if (0.f != m_vForce.Length())
	{
		// 크기
		//float fForce = m_vForce.Length();

		// 방향 (참고: Nomalize()는 원본을 직접 변경함)
		//m_vForce.Nomalize();

		// 가속도 (A = F/M)
		//float m_fAccel = fForce / m_fMass;

		// 가속도 벡터
		m_vAccel = m_vForce / m_fMass * fDT;

		// 속도
		m_vVelocity += m_vAccel;	
	}

	// 속도에 따른 이동
	Move();
	// 힘 초기화
	m_vForce = Vec2(0.f, 0.f);
}
void CRigidBody::Move()
{
	Vec2 vPos = m_pOwner->GetPos();

	// 이동 속력
	/*
	float fSpeed = m_vVelocity.Length();

	Vec2 vPos = m_pOwner->GetPos();

	Vec2 vDir = m_vVelocity;
	vDir.Nomalize();

	vPos += vDir * fSpeed * fDT;
	*/

	if (!m_vVelocity.isZero())
	{
		vPos += m_vVelocity * fDT;
		m_pOwner->SetPos(vPos);
	}

}