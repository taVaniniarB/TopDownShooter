#include "pch.h"
#include "CCollider.h"

#include "CObject.h"
#include "CCore.h"
#include "CCore.h"

#include "SelectGDI.h"

UINT CCollider::g_iNextID = 0;

CCollider::CCollider()
	: m_pOwner(nullptr)
	// collider ���� ������ id�� 1 �����ؼ� ��ĥ �� ���� ��
	, m_iID(g_iNextID++)
	, m_iCol(0)
	, m_bActive(true)
{
}

// �浹ü�� ���� �����ڴ� ID ��ħ ������,
// ����� ��ü�� �Ҽ� ������ ���� ��ü������ �����Ѵ�
CCollider::CCollider(const CCollider& _origin)
	: m_pOwner(nullptr) // ����� �浹ü�� �Ҽ��� null�� ���ش�(�Ҽ��� ������)
	, m_vOffsetPos(_origin.m_vOffsetPos)
	//, m_vFinalPos(_origin.m_vFinalPos) �˾Ƽ� ���ɰŶ� ����� �ʿ� ����
	, m_vScale(_origin.m_vScale)
	, m_iID(g_iNextID++)
	, m_iCol(0)
	, m_bActive(_origin.m_bActive)
{
}

CCollider::~CCollider()
{
}

void CCollider::finalUpdate()
{
	// ���� ��ǥ�μ� ���� ���� �� final pos ����
	m_vPrevPos = m_vFinalPos;

	// ������Ʈ�� ��ġ�� ���󰣴�

	Vec2 vObjectPos = m_pOwner->GetPos();
	
	m_vFinalPos = vObjectPos + m_vOffsetPos;

	assert(0 <= m_iCol);
}

void CCollider::render(HDC _dc)
{
	PEN_TYPE ePen = PEN_TYPE::GREEN;

	if (m_iCol)
	{
		ePen = PEN_TYPE::RED;
	}

	// ��ü�� �����ڸ� �̿��ϴ� ���
	SelectGDI p(_dc, ePen);
	SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vFinalPos);
	// FinalPos�� ����ǥ ����̶� �ٲ�� �ȵ�

	Rectangle(_dc
		, (int)(vRenderPos.x - m_vScale.x / 2.f)
		, (int)(vRenderPos.y - m_vScale.y / 2.f)
		, (int)(vRenderPos.x + m_vScale.x / 2.f)
		, (int)(vRenderPos.y + m_vScale.y / 2.f));

}

void CCollider::OnCollision(CCollider* _pOther)
{
	m_pOwner->OnCollision(_pOther);
}

void CCollider::OnCollisionEnter(CCollider* _pOther)
{
	++m_iCol;
	m_pOwner->OnCollisionEnter(_pOther);
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
	m_pOwner->OnCollisionExit(_pOther);
	--m_iCol;
}
