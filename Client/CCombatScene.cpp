#include "pch.h"
#include "CCombatScene.h"
#include "CTimeMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"

#include "CStage.h"

CCombatScene::CCombatScene()
	: m_iCombo(0)
	, m_iScore(0)
	, m_fCurTime(0)
	, m_fComboTime(0)
{
}

CCombatScene::~CCombatScene()
{
}

void CCombatScene::update()
{// ���� ���� ��ȸ, HP üũ
	const vector<CObject*>& vecMonster = CSceneMgr::GetInst()->GetCurScene()->GetGroupObject(GROUP_TYPE::MONSTER);
	for (size_t i = 0; vecMonster.size() > i; ++i)
	{
		// ���� ���� > DeleteObject ���ϱ� ���� ����
		if (vecMonster[i]->IsDead())
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
void CCombatScene::AddScore()
{
	AddCombo();

	// �� ó�� �� ����
	// �⺻ 100�� + ���� �޺��� 50�����ŭ �����Ѵ�.
	CStage::GetInst()->AddScore(100 + (m_iCombo * 50));
}

void CCombatScene::AddCombo()
{
	++m_iCombo;
	m_fCurTime = 0;
}

// ��� ������ ���� �����ϰ� �ֱ�
//	���� ���� ��ȸ�ϸ� HP�� 0�� �� ������ AddCombo�� AddScore ȣ��
