#include "pch.h"
#include "CPlayer.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CMissile.h"

#include "CResMgr.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

CPlayer::CPlayer()
{
	// �ϴ� ������Ʈ�� �θ��� private ����̴�
	// �ٵ� ������Ʈ �ʿ��� �ڽ� �ʿ��� ���� ���������� �ϴϱ�
	// �ڽ��ʿ��� �θ𿡱����� ������Ʈ ���� �Լ�(Create)�� ȣ���Ͽ� ������Ʈ�� �����ϰ�
	// ������Ʈ�� ����� �̿��� �� Getter ����Ͽ� �ش� ������Ʈ �ּҸ� �޾ƿ� ����
	// ������ �������� �Լ��� �����Ѵ�

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(Vec2(30.f, 55.f));


	// Texture �ε��ϱ�
	CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\Fif.bmp");
	
	CreateAnimator();
	GetAnimator()->CreateAnimation(L"IDLE", pTex, Vec2(0.f, 0.f), Vec2(36.f, 40.f), Vec2(36.f, 0.f), 0.1f, 12);
	// Animator �ȿ� �ִ� IDLE�̶�� Animation�� �� �ؽ�ó�� �����ϰ� �̷� �������� �������� ��
	// ���ϴ� ����� ����ϵ��� �Ѵ� (����, �÷��̾�� �ؽ�ó ����� ���� �ʿ䰡 ������)
	GetAnimator()->Play(L"IDLE", true);

	CAnimation* pAnim = GetAnimator()->FindAnimation(L"IDLE");

	for (UINT i = 0; i < pAnim->GetMaxFrame(); ++i)
	{
		pAnim->GetFrame(i).vOffset = Vec2(0.f, -20.f);
	}
}

CPlayer::~CPlayer()
{
}



void CPlayer::update()
{
	Vec2 vPos = GetPos();

	if (KEY_HOLD(KEY::W))
	{
		vPos.y -= 200.f * fDT;
	}
	if (KEY_HOLD(KEY::S))
	{
		vPos.y += 200.f * fDT;
	}
	if (KEY_HOLD(KEY::A))
	{
		vPos.x -= 200.f * fDT;
	}
	if (KEY_HOLD(KEY::D))
	{
		vPos.x += 200.f * fDT;
	}
	if (KEY_TAP(KEY::SPACE))
	{
		CreateMissile();
	}

	SetPos(vPos);

	GetAnimator()->update();
}
void CPlayer::render(HDC _dc)
{
	component_render(_dc);
}
void CPlayer::CreateMissile()
{
	Vec2 MissilePos = GetPos();
	MissilePos.y -= GetScale().y / 2.f;

	//�̻��� ��ü
	CMissile* pMissile = new CMissile;
	pMissile->SetName(L"Player_Missile");
	pMissile->SetPos(MissilePos);
	pMissile->SetScale(Vec2(25.f, 25.f));
	pMissile->SetDir(Vec2(0.f, -1.f));

	CreateObject(pMissile, GROUP_TYPE::PROJ_PLAYER);
}
