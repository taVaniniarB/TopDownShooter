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
	// ���� ���� ��
	// ���� ���� ���ӵ� ���
	if (0.f != m_vForce.Length())
	{
		// finalupdate �������� m_vForce��
		// �� �����ӿ����� ���� ����� ũ�� ������ ��� �ִ� ����
		
		// ���ӵ� ����
		m_vAccel = m_vForce / m_fMass;
	}


	// �߰� ���ӵ� �����ֱ�
	// ���� �ֵ� ���� �߰� ���ӵ��� ���ǵ��� �Ѵ�.
	m_vAccel += m_vAccelA;

	// �ӵ�
	m_vVelocity += m_vAccel * fDT;



	// ���� ���� > �ִ�ӵ����� ���� (�׷��� �ִ�ӵ��� ���� ������)

	// �����¿� ���� �ݴ� ���������� ���ӵ�
	if (!m_vVelocity.isZero())
	{
		Vec2 vDir = m_vVelocity;
		vDir.Normalize();

		Vec2 vFriction = -vDir * m_fFricCoeff * fDT;

		if (vFriction.Length() >= m_vVelocity.Length())
		{// ���� ���ӵ��� ���� �ӵ����� ū ���
			m_vVelocity = Vec2(0.f, 0.f);
		}
		else
		{
			m_vVelocity += vFriction;
		}
	}

	// ž�� �ӵ� ���� �˻�
	if (m_fMaxVelocity < m_vVelocity.Length())
	{
		// �ִ� �ӵ��� �Ѱ��� ��
		// �ӵ��� �������Ϳ� �ִ�ӵ��� ���ؼ�, ������ ������ �����ϸ� �� �ִ�ӷ��� ���ͷ� ������ش�
		Vec2 vDir = m_vVelocity;
		if (!m_vVelocity.isZero())
		{
			vDir.Normalize();
		}
		m_vVelocity = vDir * m_fMaxVelocity;
	}
	//
	//// Ⱦ��ũ�� �ӵ� ���� �˻� (x, y ���� �ִ�ӷ��� ����)
	//if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x))
	//{
	//	// �ִ� �ӵ��� �Ѱ��� ��
	//	// ���� ���⿡ �ִ� �ӵ��� ���ؼ�, ������ ������ �����ϸ� �� �ִ�ӷ��� ���ͷ� ������ش�

	//	// ���� (-1, 1)
	//	m_vVelocity.x = m_vVelocity.x / abs(m_vVelocity.x) * abs(m_vMaxVelocity.x);
	//}
	//if (abs(m_vMaxVelocity.y) < abs(m_vVelocity.y))
	//{
	//	// ���� (-1, 1)
	//	m_vVelocity.y = m_vVelocity.y / abs(m_vVelocity.y) * abs(m_vMaxVelocity.y);
	//}

	// �ӵ��� ���� �̵�
	Move();

	// �� �ʱ�ȭ
	m_vForce = Vec2(0.f, 0.f);

	// ���ӵ� �ʱ�ȭ
	m_vAccel = Vec2(0.f, 0.f);

	// �߰� ���ӵ� �ʱ�ȭ
	// �̰� �� ���ָ� �߰����ӵ��� �����ż�,
	// �������� ���鿡 ���� ���Ŀ��� �߰� ���ӵ��� ��� �����ϴ� ���°� �Ź���
	m_vAccelA = Vec2(0.f, 0.f);
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