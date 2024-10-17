#include "pch.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CObject.h"


CRigidBody::CRigidBody()
	: m_pOwner(nullptr)
	, m_fMass(1.f)
	, m_fFricCoeff(1000.f)
	, m_fMaxVelocity(400.f)
	, m_bGround(false)
	, m_eGroundWallDir(WALL_DIR::END)
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::finalUpdate()
{
	// 힘이 있을 때
	// 힘에 따른 가속도 계산
	if (0.f != m_vForce.Length())
	{
		// finalupdate 시점에서 m_vForce는
		// 그 프레임에서의 최종 방향과 크기 정보가 담겨 있는 상태
		
		// 가속도 벡터
		m_vAccel = m_vForce / m_fMass;
	}


	// 추가 가속도 더해주기
	// 힘이 있든 없든 추가 가속도는 계산되도록 한다.
	m_vAccel += m_vAccelA;

	// 속도
	m_vVelocity += m_vAccel * fDT;



	// 마찰 적용 > 최대속도제한 순서 (그래야 최대속도에 도달 가능함)

	// 마찰력에 의한 반대 방향으로의 가속도
	if (!m_vVelocity.isZero())
	{
		Vec2 vDir = m_vVelocity;
		vDir.Normalize();

		Vec2 vFriction = -vDir * m_fFricCoeff * fDT;

		if (vFriction.Length() >= m_vVelocity.Length())
		{// 마찰 가속도가 본래 속도보다 큰 경우
			m_vVelocity = Vec2(0.f, 0.f);
		}
		else
		{
			m_vVelocity += vFriction;
		}
	}

	// 탑뷰 속도 제한 검사
	if (m_fMaxVelocity < m_vVelocity.Length())
	{
		// 최대 속도를 넘겼을 시
		// 속도의 단위벡터에 최대속도를 곱해서, 기존의 방향을 유지하며 딱 최대속력인 벡터로 만들어준다
		Vec2 vDir = m_vVelocity;
		if (!m_vVelocity.isZero())
		{
			vDir.Normalize();
		}
		m_vVelocity = vDir * m_fMaxVelocity;
	}
	//
	//// 횡스크롤 속도 제한 검사 (x, y 따로 최대속력이 존재)
	//if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x))
	//{
	//	// 최대 속도를 넘겼을 시
	//	// 기존 방향에 최대 속도를 곱해서, 기존의 방향을 유지하며 딱 최대속력인 벡터로 만들어준다

	//	// 방향 (-1, 1)
	//	m_vVelocity.x = m_vVelocity.x / abs(m_vVelocity.x) * abs(m_vMaxVelocity.x);
	//}
	//if (abs(m_vMaxVelocity.y) < abs(m_vVelocity.y))
	//{
	//	// 방향 (-1, 1)
	//	m_vVelocity.y = m_vVelocity.y / abs(m_vVelocity.y) * abs(m_vMaxVelocity.y);
	//}

	// 속도에 따른 이동
	Move();

	// 힘 초기화
	m_vForce = Vec2(0.f, 0.f);

	// 가속도 초기화
	m_vAccel = Vec2(0.f, 0.f);

	// 추가 가속도 초기화
	// 이거 안 해주면 추가가속도가 누적돼서,
	// 떨어져서 지면에 닿은 이후에도 추가 가속도가 계속 존재하는 상태가 돼버림
	m_vAccelA = Vec2(0.f, 0.f);
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