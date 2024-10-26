#include "pch.h"
#include "CHitbox.h"
#include "CCollider.h"
#include "CPlayer.h"
#include "CMonster.h"

CHitbox::CHitbox()
	: m_pOwner(nullptr)
{
	CreateCollider();
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
	//component_render(_dc);
}

void CHitbox::OnCollision(CCollider* _pOther)
{
}

void CHitbox::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	wstring otherObjName = pOtherObj->GetName();
	wstring ownerName = m_pOwner->GetName();



	// 오너가 플레이어 (플레이어 히트박스가 맞음)
	if (ownerName == L"Player")
	{
		if (otherObjName == L"KnifeDA")
		{

		}
		else if (otherObjName == L"Missle")

		((CPlayer*)m_pOwner)->SubHP();
	}

	// 오너가 몬스터 (몬스터 히트박스가 맞음)
	else if (ownerName == L"Monster")
	{
		if (otherObjName == L"KnifeDA")
		{
			((CMonster*)m_pOwner)->SubHP(L"Knife");
		}
		else if (otherObjName == L"PipeDA")
		{
			((CMonster*)m_pOwner)->SubHP(L"Pipe");
		}
		else if (otherObjName == L"Missile")
		{
			((CMonster*)m_pOwner)->SubHP(L"Missile");
		}
	}
}