#include "pch.h"
#include "CScoreMgr.h"
#include "CTimeMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"

CScoreMgr::CScoreMgr()
	: m_iCombo(0)
	, m_iScore(0)
	, m_fCurTime(0)
	, m_fComboTime(0)
{
	// �� óġ�κ��� 1�� ���� ���� óġ�ؾ� �޺��� �����Ѵ�.
	// óġ���� ���ϸ� �޺��� ���µȴ�.
	m_fComboTime = 1.f;
}

CScoreMgr::~CScoreMgr()
{
}

void CScoreMgr::update()
{
	// ���� ���� ��ȸ, HP üũ
	const vector<CObject*>& vecMonster = CSceneMgr::GetInst()->GetCurScene()->GetGroupObject(GROUP_TYPE::MONSTER);
	for (size_t i = 0; vecMonster.size() > i; ++i)
	{
		// ���� ���� > DeleteObject ���ϱ� ���� ����
		if (0 == vecMonster[i]->IsDead())
		{
			AddScore();
		}
	}
	
	m_fCurTime += fDT;

	if (m_fCurTime > m_fComboTime)
	{
		ResetCombo();
	}
}

// ������ HP�� 0�� ���� �� ȣ��
void CScoreMgr::AddScore()
{
	AddCombo();

	// �� ó�� �� ����
	// �⺻ 100�� + ���� �޺��� 50�����ŭ �����Ѵ�.
	m_iScore += 100 + (m_iCombo * 50);

	// �׷� �ð� ��
	// ���� ���̸� AddCombo ���� ����ž� ��.
}

void CScoreMgr::AddCombo()
{
	++m_iCombo;
	m_fCurTime = 0;
}

// ��� ������ ���� �����ϰ� �ֱ�
//	���� ���� ��ȸ�ϸ� HP�� 0�� �� ������ AddCombo�� AddScore ȣ��
// or
// ��� ������ Update���� HP�� 0�� �Ǹ� ȣ�����ֱ�
