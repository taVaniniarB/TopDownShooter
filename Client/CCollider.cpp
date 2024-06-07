#include "pch.h"
#include "CCollider.h"

#include "CObject.h"
#include "CCore.h"
#include "CCore.h"

#include "SelectGDI.h"

UINT CCollider::g_iNextID = 0;

CCollider::CCollider()
	: m_pOwner(nullptr)
	// collider 생성 때마다 id가 1 증가해서 겹칠 수 없게 됨
	, m_iID(g_iNextID++)
	, m_iCol(0)
{
}

// 충돌체의 복사 생성자는 ID 겹침 문제와,
// 복사된 객체의 소속 문제로 인해 자체적으로 구현한다
CCollider::CCollider(const CCollider& _origin)
	: m_pOwner(nullptr) // 복사된 충돌체의 소속은 null로 해준다(소속이 없도록)
	, m_vOffsetPos(_origin.m_vOffsetPos)
	//, m_vFinalPos(_origin.m_vFinalPos) 알아서 계산될거라 명시할 필요 없음
	, m_vScale(_origin.m_vScale)
	, m_iID(g_iNextID++)
{
}

CCollider::~CCollider()
{
}

void CCollider::finalUpdate()
{
	// 오브젝트의 위치를 따라간다

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

	// 객체의 생성자를 이용하는 방식
	SelectGDI p(_dc, ePen);
	SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vFinalPos);
	// FinalPos는 찐좌표 기반이라 바뀌면 안됨

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
