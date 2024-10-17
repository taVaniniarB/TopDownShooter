#include "pch.h"
#include "CTraceState.h"

#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CScene.h"

#include "CMonster.h"

#include "CTimeMgr.h"

CTraceState::CTraceState()
	: CState(MON_STATE::TRACE)
{
}

CTraceState::~CTraceState()
{
}

void CTraceState::update()
{
	// Ÿ���� �� Player�� �Ѿư���
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlaterPos = pPlayer->GetPos();
	Vec2 vMonPos = GetMonster()->GetPos();

	// ���� > �÷��̾� ���� ���� ���
	Vec2 vMonDir = vPlaterPos - vMonPos;
	if (vMonDir.Length() < 20)
		return;

	vMonDir.Normalize();
	
	// �̹� �������� �̵����� �����ش�
	// �������Ϳ� ��Į�� �� --> ���������� ���̰� ��Į�� ����ŭ �þ
	vMonPos += vMonDir * GetMonster()->GetInfo().fSpeed * fDT;
	
	GetMonster()->SetPos(vMonPos);
}

void CTraceState::Enter()
{
}

void CTraceState::Exit()
{
}

