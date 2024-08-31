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
	// finalupdate �������� m_vForce��
	// �� �����ӿ����� ���� ����� ũ�� ������ ��� �ִ� ����
	// ���ӵ��� ���غ���.
	
	if (0.f != m_vForce.Length())
	{
		// ũ��
		//float fForce = m_vForce.Length();

		// ���� (����: Nomalize()�� ������ ���� ������)
		//m_vForce.Nomalize();

		// ���ӵ� (A = F/M)
		//float m_fAccel = fForce / m_fMass;

		// ���ӵ� ����
		m_vAccel = m_vForce / m_fMass * fDT;

		// �ӵ�
		m_vVelocity += m_vAccel;	
	}

	// �ӵ��� ���� �̵�
	Move();
	// �� �ʱ�ȭ
	m_vForce = Vec2(0.f, 0.f);
}
void CRigidBody::Move()
{
	Vec2 vPos = m_pOwner->GetPos();

	// �̵� �ӷ�
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