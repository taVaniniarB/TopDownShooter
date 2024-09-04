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
#include "CGravity.h"

#include "CRigidBody.h"

CPlayer::CPlayer()
	: m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::WALK)
	, m_iDir(1)
	, m_iPrevDir(1)
{
	// �ϴ� ������Ʈ�� �θ��� private ����̴�
	// �ٵ� ������Ʈ �ʿ��� �ڽ� �ʿ��� ���� ���������� �ϴϱ�
	// �ڽ��ʿ��� �θ𿡱����� ������Ʈ ���� �Լ�(Create)�� ȣ���Ͽ� ������Ʈ�� �����ϰ�
	// ������Ʈ�� ����� �̿��� �� Getter ����Ͽ� �ش� ������Ʈ �ּҸ� �޾ƿ� ����
	// ������ �������� �Լ��� �����Ѵ�

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 3.f));
	GetCollider()->SetScale(Vec2(20.f, 35.f));
	

	CreateRigidBody();

	// Texture �ε��ϱ�
	//CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\ray_t.bmp");
	CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\Fif.bmp");

	CreateAnimator();
	GetAnimator()->CreateAnimation(L"IDLE_RIGHT", pTex, Vec2(0.f, 0.f), Vec2(36.f, 40.f), Vec2(36.f, 0.f), 0.1f, 12);
	GetAnimator()->CreateAnimation(L"IDLE_LEFT", pTex, Vec2(0.f, 0.f), Vec2(36.f, 40.f), Vec2(36.f, 0.f), 0.1f, 12);
	
	GetAnimator()->CreateAnimation(L"RUN", pTex, Vec2(0.f, 52.f), Vec2(46.f, 38.f), Vec2(46.f, 0.f), 0.1f, 10);
	//GetAnimator()->CreateAnimation(L"RUN", pTex, Vec2(0.f, 0.f), Vec2(36.f, 40.f), Vec2(36.f, 0.f), 0.1f, 12);
	// Animator �ȿ� �ִ� IDLE�̶�� Animation�� �� �ؽ�ó�� �����ϰ� �̷� �������� �������� ��
	// ���ϴ� ����� ����ϵ��� �Ѵ� (����, �÷��̾�� �ؽ�ó ����� ���� �ʿ䰡 ������)

	GetAnimator()->CreateAnimation(L"JUMP", pTex, Vec2(0.f, 135.f), Vec2(36.f, 45.f), Vec2(36.f, 0.f), 0.1f, 8);
	
	CreateGravity();


	// �ִϸ��̼ǿ� �������� �ִ� �ڵ�
	/*
	CAnimation* pAnim = GetAnimator()->FindAnimation(L"IDLE_RIGHT");

	for (UINT i = 0; i < pAnim->GetMaxFrame(); ++i)
	{
		pAnim->GetFrame(i).vOffset = Vec2(0.f, -20.f);
	}*/
}

CPlayer::~CPlayer()
{
	
}



void CPlayer::update()
{
	update_move();

	// ���� ������Ʈ
	update_state();

	update_attack();

	// ������Ʈ�� ���¿� ���� �ִϸ��̼� ������Ʈ
	update_animation();

	
	if (KEY_TAP(KEY::ENTER))
	{
		SetPos(Vec2(640.f, 320.f));
	}

	GetAnimator()->update();

	// �� ù�� ���� �������� �־��ұ�...
	// �߰��� �ǰ� ���°� �Ǿ��� ����
	// �̰� ù �ٿ� ���� prev�� cur�� �������� update_state �Լ� ����ó���� �ɷ��� ���ϵŹ���
	m_ePrevState = m_eCurState;
	m_iPrevDir = m_iDir;
}
void CPlayer::render(HDC _dc)
{
	// ������Ʈ(�浹ü, etc,,) �ִ� ��� ����
	component_render(_dc);

	// Alpha Blending Test

	//CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\rat_t.bmp");

	//Vec2 vPos = GetPos();
	//vPos = CCamera::GetInst()->GetRenderPos(vPos);

	//float width = (float)pTex->Width();
	//float height = (float)pTex->Height();

	//BLENDFUNCTION bf = {};
	//bf.BlendOp = AC_SRC_OVER;
	//bf.AlphaFormat = AC_SRC_ALPHA;
	//bf.BlendFlags = 0;
	//bf.SourceConstantAlpha = 127; //���������� ����Ǵ� ����!

	//// ������ dc, �����ġ(�»���� x, y), ���μ���, ���� dc, ���� �̹����� �»�� xy, �ʺ����, BLANDFUNCTION ����ü
	//AlphaBlend(_dc
	//	, int(vPos.x - width / 2.f)
	//	, int(vPos.y - height / 2.f)
	//	, int(width), int(height)
	//	, pTex->GetDC()
	//	, 0, 0, (int)width, (int)height, bf);

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

void CPlayer::update_state()
{
	if (KEY_HOLD(KEY::A) && GetGravity()->GetGround())
	{
		m_iDir = -1;
		if (PLAYER_STATE::JUMP != m_eCurState)
			m_eCurState = PLAYER_STATE::RUN;
	}
	if (KEY_HOLD(KEY::D) && GetGravity()->GetGround())
	{
		m_iDir = 1;
		if (PLAYER_STATE::JUMP != m_eCurState)
			m_eCurState = PLAYER_STATE::RUN;
	}
	/*if (KEY_TAP(KEY::SPACE))
	{
		m_eCurState = PLAYER_STATE::ATTACK;
	}*/
	// Ű�� ������ �ʾƵ� �ӵ��� ���� ���� ���� ����� ���Ǽ���
	if (0.f == GetRigidBody()->GetSpeed() && PLAYER_STATE::JUMP != m_eCurState)
	{
		m_eCurState = PLAYER_STATE::IDLE;
	}
	/*
	if (KEY_NONE(KEY::A) && KEY_NONE(KEY::D)&& PLAYER_STATE::JUMP != m_eCurState)
	{
		m_eCurState = PLAYER_STATE::IDLE;
	}*/
	if (KEY_TAP(KEY::W))
	{
		if (m_eCurState != PLAYER_STATE::JUMP)
		{
			m_eCurState = PLAYER_STATE::JUMP;

			if (GetRigidBody())
			{
				GetRigidBody()->AddVelocity(Vec2(0.f, -500.f));
			}
		}

		
	}
}

void CPlayer::update_move()
{
	CRigidBody* pRigid = GetRigidBody();

	Vec2 vPlayerMoveVec(0.f, 0.f);

	Vec2 vPos = GetPos();

	//if (KEY_HOLD(KEY::S))
	//{
	//	vPlaterMoveVec += Vec2(0.f, 200.f);
	//}
	if (KEY_HOLD(KEY::A))
	{
		vPlayerMoveVec += Vec2(-200.f, 0.f);
	}
	if (KEY_HOLD(KEY::D))
	{
		vPlayerMoveVec += Vec2(200.f, 0.f);
	}


	if (!vPlayerMoveVec.isZero())
	{
		vPlayerMoveVec.Nomalize();
		pRigid->AddForce(vPlayerMoveVec * 200.f);
	}


	if (KEY_TAP(KEY::W))
	{
	}
	/*if (KEY_TAP(KEY::S))
	{
		pRigid->AddVelocity(Vec2(0.f, 100.f));
	}*/
	if (KEY_TAP(KEY::A))
	{// �� �����Ӱ� ���� ���⿡�� ������ �� �� ���ӵ��� ����Ǹ� �� ��
		pRigid->SetVelocity(Vec2(-150.f, pRigid->GetVelocity().y));
	}
	if (KEY_TAP(KEY::D))
	{
		pRigid->SetVelocity(Vec2(150.f, pRigid->GetVelocity().y));
	}

	SetPos(vPos);
}

void CPlayer::update_attack()
{
	switch (m_eCurState)
	{
	case PLAYER_STATE::ATTACK:
		CreateMissile();
		break;
	}
	
}

void CPlayer::update_animation()
{
	// ���¿� ������ ��� ������ ���ٸ� return
	if (m_ePrevState == m_eCurState && m_iDir == m_iPrevDir)
		return;


	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
	{
		if (m_iDir == -1)
			GetAnimator()->Play(L"IDLE_LEFT", true);
		else
			GetAnimator()->Play(L"IDLE_RIGHT", true);
	}
		break;
	case PLAYER_STATE::WALK:
		if (m_iDir == -1)
			GetAnimator()->Play(L"WALK_LEFT", true);
		else
			GetAnimator()->Play(L"WALK_RIGHT", true);
		break;
	case PLAYER_STATE::RUN:
		if (m_iDir == -1)
			GetAnimator()->Play(L"RUN", true);
		else
			GetAnimator()->Play(L"RUN", true);
		break;
	case PLAYER_STATE::JUMP:
		if (m_iDir == -1)
			GetAnimator()->Play(L"JUMP", true);
		else
			GetAnimator()->Play(L"JUMP", true);
		break;
	case PLAYER_STATE::ATTACK:
		break;
	case PLAYER_STATE::DEAD:
		break;
	default:
		break;
	}
}

void CPlayer::update_gravity()
{
	// �ٴڿ� �پ� �ִ� ���� = �̹� �߷� '���' ���̹Ƿ� ���ӵ��� ���� ������ ����
	// �׷��ٰ� ����� �� ���°� ���� ��, �Ʒ��� ���ϴ� ���ӵ��� �ٿ��ش�
	
	if (!GetGravity()->GetGround())
	{
		GetRigidBody()->SetAccelAlpha(Vec2(0.f, 500.f));
	}

	// ������ �ؼ� ���߿� �ߴ� �� '����'���� �߷� �����
	// ������ ������ ���� '��'�� �� ��...������
	// ������ �������, ���� �������� �ٷ� '�ӵ�'�� ��

	// �߷��� ���� ���ӵ��� �ο��Ѵ�
	// �� ������ ���ӵ��� �����Ų��
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Ground")
	{
		Vec2 vPos = GetPos();

		// Ground���� Player ��ǥ�� �� ����, �� Ground ���� �ö��� ��
		if (vPos.y < pOtherObj->GetPos().y)
		{
			m_eCurState = PLAYER_STATE::IDLE;
		}
	}
}

