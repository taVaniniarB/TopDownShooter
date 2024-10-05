#include "pch.h"
#include "CObject.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CGravity.h"

CObject::CObject()
	: m_vPos{}
	, m_vScale{}
	, m_pTex{ nullptr }
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidBody(nullptr)
	, m_pGravity(nullptr)
	, m_bAlive(true)
	, m_bEnabled(true)
{
}

CObject::CObject(const CObject& _origin)
	: m_strName(_origin.m_strName)
	, m_vPos(_origin.m_vPos)
	, m_vScale(_origin.m_vScale)
	, m_pTex(_origin.m_pTex)
	, m_iImgIdx(_origin.m_iImgIdx)
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidBody(nullptr)
	, m_pGravity(nullptr)
	, m_bAlive(true)
	, m_bEnabled(true)
{
	//컴포넌트의 깊은 복사 구현
	if (_origin.m_pCollider)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->m_pOwner = this;
	}
	// 의문: 충돌체 자식 객체에서 또 만들어지지 않나?
	// '복사'라는 상황
	// 오브젝트의 복사 생성자 실행 > 자식의 복사 생성자 실행
	// 이라 생각하면, 오브젝트 복사 생성자에서 컴포넌트가 만들어지고
	// 자식의 복사 생성자에서 CreateCollider 실행 안하기 때문에
	// 중복생성 안 됨
	// 컴포넌트는 Object의 멤버로 관리되고 있기 때문에
	// 자식의 복사 생성자에 따로 생성 구문을 적지 않는 이상, 복사하여도 자식 쪽에서 생성되지 않음
	// 각각 자기의 멤버만을 복사하는 것
	
	if (_origin.m_pAnimator)
	{
		m_pAnimator = new CAnimator(*_origin.m_pAnimator);
		m_pAnimator->m_pOwner = this;
	}

	if (_origin.m_pRigidBody)
	{
		m_pRigidBody = new CRigidBody(*_origin.m_pRigidBody);
		m_pRigidBody->m_pOwner = this;
	}

	if (_origin.m_pGravity)
	{
		m_pGravity = new CGravity(*_origin.m_pGravity);
		m_pGravity->m_pOwner = this;
	}
}

CObject::~CObject()
{
	if (nullptr != m_pCollider)
	{
		delete m_pCollider;
	}

	if (nullptr != m_pAnimator)
	{
		delete m_pAnimator;
	}

	if (nullptr != m_pRigidBody)
	{
		delete m_pRigidBody;
	}

	if (nullptr != m_pGravity)
	{
		delete m_pGravity;
	}
}


void CObject::finalUpdate()
{
	if (!m_bEnabled)
		return;

	if (m_pAnimator)
		m_pAnimator->finalUpdate();

	// 중력에서 힘을 받은 후, RigidBody 컴포넌트에서 이를 계산
	if (m_pGravity)
		m_pGravity->finalUpdate();

	if (m_pRigidBody)
		m_pRigidBody->finalUpdate();


	// Collider의 FinalUpdate에서 Collider가 플레이어를 따라잡아야 하므로
	// Collider Update는 점프에 의한 좌표 세팅 작업이 이루어지는 RigidBody 뒤에 위치해야 함

	if (m_pCollider)
		m_pCollider->finalUpdate();
}

void CObject::render(HDC _dc)
{
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vPos);

	Rectangle(_dc
		, (int)(vRenderPos.x - m_vScale.x / 2.f)
		, (int)(vRenderPos.y - m_vScale.y / 2.f)
		, (int)(vRenderPos.x + m_vScale.x / 2.f)
		, (int)(vRenderPos.y + m_vScale.y / 2.f));

	component_render(_dc);
}

void CObject::component_render(HDC _dc)
{
	if (nullptr != m_pAnimator)
	{
		m_pAnimator->render(_dc);
	}

	if (nullptr != m_pCollider)
	{
		m_pCollider->render(_dc);
	}
}

void CObject::CreateCollider()
{
	m_pCollider = new CCollider;
	m_pCollider->m_pOwner = this;
}

void CObject::CreateAnimator()
{
	m_pAnimator = new CAnimator;
	m_pAnimator->m_pOwner = this;
}

void CObject::CreateRigidBody()
{
	m_pRigidBody = new CRigidBody;
	m_pRigidBody->m_pOwner = this;
}

void CObject::CreateGravity()
{
	m_pGravity = new CGravity;
	m_pGravity->m_pOwner = this;
}
