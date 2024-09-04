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

	// �� ���� ���� > RigidBody�� y���� �ӵ��� 0�� �Ǿ�� ��
	if (m_bGround)
	{
		Vec2 vV = m_pOwner->GetRigidBody()->GetVelocity();
		m_pOwner->GetRigidBody()->SetVelocity(Vec2(vV.x, 0.f));
	}
}


void CGravity::finalUpdate()
{// �� ������ �̰��� ���� ��..
	// ������ �����ϴ� ��ü�� �ϴÿ� �ִ��� ���� �ִ��� �Ǻ��ؾ� �Ѵ�
	// �� > �浹ü
	if (!m_bGround)
	{
		// �߷°��ӵ��� ������ ������ �ſ� ũ�� ������ ���� ���ӵ�
		// ��ü�� ���� ����Ǿ� ������, �� ���� ������ ���� ���ӵ���, �߷°��ӵ��� �÷����� ���ؾ� ��
		// �Ϲ����� '�� �ο��ϱ�'�� ��Ŀ������ �ٸ���.
		// ���� ���� ���ӵ� + �߰� ���ӵ� (�߷�)
		m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, 800.f));
	}
}