#include "pch.h"
#include "CObject.h"

#include "CCollider.h"
#include "CAnimator.h"

CObject::CObject()
	: m_vPos{}
	, m_vScale{}
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_bAlive(true)
{
}

CObject::CObject(const CObject& _origin)
	: m_strName(_origin.m_strName)
	, m_vPos(_origin.m_vPos)
	, m_vScale(_origin.m_vScale)
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_bAlive(true)
{
	//������Ʈ�� ���� ���� ����
	if (_origin.m_pCollider)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->m_pOwner = this;
	}
	// �ǹ�: �浹ü �ڽ� ��ü���� �� ��������� �ʳ�?
	// '����'��� ��Ȳ
	// ������Ʈ�� ���� ������ ���� > �ڽ��� ���� ������ ����
	// �̶� �����ϸ�, ������Ʈ ���� �����ڿ��� ������Ʈ�� ���������
	// �ڽ��� ���� �����ڿ��� CreateCollider ���� ���ϱ� ������
	// �ߺ����� �� ��
	// ������Ʈ�� Object�� ����� �����ǰ� �ֱ� ������
	// �ڽ��� ���� �����ڿ� ���� ���� ������ ���� �ʴ� �̻�, �����Ͽ��� �ڽ� �ʿ��� �������� ����
	// ���� �ڱ��� ������� �����ϴ� ��
	
	if (_origin.m_pAnimator)
	{
		m_pAnimator = new CAnimator(*_origin.m_pAnimator);
		m_pAnimator->m_pOwner = this;
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
}


void CObject::finalUpdate()
{
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
	if (nullptr != m_pCollider)
	{
		m_pCollider->render(_dc);
	}

	if (nullptr != m_pAnimator)
	{
		m_pAnimator->render(_dc);
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