#include "pch.h"
#include "CHitbox.h"
#include "CCollider.h"
#include "CPlayer.h"
#include "CMonster.h"

CHitbox::CHitbox(Vec2 _vColliderScale)
	: m_pOwner(nullptr)
{
	CreateCollider();
	GetCollider()->SetScale(_vColliderScale);
}

CHitbox::~CHitbox()
{
}

void CHitbox::update()
{
	if (!m_pOwner)
		DeleteObject(this);

	Vec2 vObjectPos = m_pOwner->GetPos();
	SetPos(vObjectPos);
}

void CHitbox::render(HDC _dc)
{
	component_render(_dc);
}

void CHitbox::OnCollision(CCollider* _pOther)
{
}

void CHitbox::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	wstring otherObjName = pOtherObj->GetName();
	wstring ownerName = m_pOwner->GetName();



	// ���ʰ� �÷��̾� (�÷��̾� ��Ʈ�ڽ��� ����)
	if (ownerName == L"Player")
	{
		((CPlayer*)m_pOwner)->SubHP(otherObjName);
	}

	// ���ʰ� ���� (���� ��Ʈ�ڽ��� ����)
	else if (ownerName == L"Monster")
	{
		((CMonster*)m_pOwner)->SubHP(otherObjName);
	}
}