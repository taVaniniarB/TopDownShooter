#include "CGravity.h"

#include "CObject.h"
#include "CRigidBody.h"


CGravity::CGravity()
	: m_pOwner(nullptr)
	, m_bGround(false)
{
}

CGravity::~CGravity()
{
}



void CGravity::SetGround(bool _b)
{
	m_bGround = _b;

	// 땅 위에 섰다 > RigidBody의 y방향 속도가 0이 되어야 함
	if (m_bGround)
	{
		Vec2 vV = m_pOwner->GetRigidBody()->GetVelocity();
		m_pOwner->GetRigidBody()->SetVelocity(Vec2(vV.x, 0.f));
	}
}


void CGravity::finalUpdate()
{// 매 프레임 이곳에 들어올 것..
	// 본인을 소유하는 물체가 하늘에 있는지 땅에 있는지 판별해야 한다
	// 땅 > 충돌체
	if (!m_bGround)
	{
		// 중력가속도는 지구의 질량이 매우 크기 때문에 고정 가속도
		// 강체에 힘이 적용되어 있으면, 그 힘에 질량을 나눈 가속도에, 중력가속도를 플러스로 더해야 함
		// 일반적인 '힘 부여하기'와 메커니즘이 다르다.
		// 힘에 의한 가속도 + 추가 가속도 (중력)
		m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, 800.f));
	}
}