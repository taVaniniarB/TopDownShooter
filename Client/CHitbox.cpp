#include "pch.h"
#include "CHitbox.h"
#include "CCollider.h"
#include "CPlayer.h"
#include "CMonster.h"

CHitbox::CHitbox()
	: m_pOwner(nullptr)
{
	CreateCollider();
	//GetCollider()->SetOffsetPos(Vec2(0.f, 3.f));
	GetCollider()->SetScale(Vec2(15.f, 20.f));
}

CHitbox::~CHitbox()
{
}

void CHitbox::update()
{
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
	// ���ʰ� �÷��̾� (�÷��̾� ��Ʈ�ڽ��� �Ѿ˿� ����)
	if (ownerName == L"Player" && otherObjName == L"Missile")
	{
		((CPlayer*)m_pOwner)->SubHP();
	}
	// ���ʰ� ���� (���� ��Ʈ�ڽ��� �Ѿ˿� ����)
	else if (ownerName == L"Monster" && otherObjName == L"Missile")
	{
		((CMonster*)m_pOwner)->SubHP();
	}
}