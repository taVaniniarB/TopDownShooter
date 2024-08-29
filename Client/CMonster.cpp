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
{// ���� �Ҹ� �� ���Ͱ� �����ϰ� �ִ� ai �Ҹ�
	if (nullptr != m_pAI)
		delete m_pAI;
}


void CMonster::update()
{
	m_pAI->update();
}

// AI - ���Ͱ� ���θ� �˵��� ��
void CMonster::SetAI(AI* _AI)
{
	m_pAI = _AI;
	_AI->m_pOwner = this;
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	// �浹�� ������Ʈ�� ��ü�� �˾Ƴ��� (�浹ü�� owner ��ȯ�ϴ� GetObj)
	CObject* pOtherObj = _pOther->GetObj();
	// obj ���� �˾Ƴ��� ���: ������Ʈ�� ���̵� �±׸� �ο�

	if (pOtherObj->GetName() == L"Player_Missile")
	{/*
		--m_iHP;

		if(m_iHP <= 0)
			DeleteObject(this);*/
	}
}