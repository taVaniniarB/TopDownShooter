#include "pch.h"
#include "CMonster.h"

#include "AI.h"

#include "CTimeMgr.h"
#include "CCollider.h"

CMonster::CMonster()
	: m_tInfo{}
	, m_pAI(nullptr)
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(40.f, 40.f));
}

CMonster::~CMonster()
{// 몬스터 소멸 시 몬스터가 보유하고 있던 ai 소멸
	if (nullptr != m_pAI)
		delete m_pAI;
}


void CMonster::update()
{
	m_pAI->update();
}

// AI - 몬스터가 서로를 알도록 함
void CMonster::SetAI(AI* _AI)
{
	m_pAI = _AI;
	_AI->m_pOwner = this;
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	// 충돌한 오브젝트의 정체를 알아내자 (충돌체의 owner 반환하는 GetObj)
	CObject* pOtherObj = _pOther->GetObj();
	// obj 종류 알아내는 방법: 오브젝트에 아이디나 태그명 부여

	if (pOtherObj->GetName() == L"Player_Missile")
	{/*
		--m_iHP;

		if(m_iHP <= 0)
			DeleteObject(this);*/
	}
}