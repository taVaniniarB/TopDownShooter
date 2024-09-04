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
	// 일단 컴포넌트는 부모의 private 멤버이다
	// 근데 컴포넌트 필요한 자식 쪽에서 직접 생성했으면 하니까
	// 자식쪽에서 부모에구현된 컴포넌트 생성 함수(Create)를 호출하여 컴포넌트를 생성하고
	// 컴포넌트의 기능을 이용할 땐 Getter 사용하여 해당 컴포넌트 주소를 받아온 다음
	// 포인터 역참조로 함수를 실행한다

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 3.f));
	GetCollider()->SetScale(Vec2(20.f, 35.f));
	

	CreateRigidBody();

	// Texture 로딩하기
	//CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\ray_t.bmp");
	CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\Fif.bmp");

	CreateAnimator();
	GetAnimator()->CreateAnimation(L"IDLE_RIGHT", pTex, Vec2(0.f, 0.f), Vec2(36.f, 40.f), Vec2(36.f, 0.f), 0.1f, 12);
	GetAnimator()->CreateAnimation(L"IDLE_LEFT", pTex, Vec2(0.f, 0.f), Vec2(36.f, 40.f), Vec2(36.f, 0.f), 0.1f, 12);
	
	GetAnimator()->CreateAnimation(L"RUN", pTex, Vec2(0.f, 52.f), Vec2(46.f, 38.f), Vec2(46.f, 0.f), 0.1f, 10);
	//GetAnimator()->CreateAnimation(L"RUN", pTex, Vec2(0.f, 0.f), Vec2(36.f, 40.f), Vec2(36.f, 0.f), 0.1f, 12);
	// Animator 안에 있는 IDLE이라는 Animation이 이 텍스처를 참조하고 이런 정보들을 참조했을 때
	// 원하는 장면을 출력하도록 한다 (따라서, 플레이어는 텍스처 멤버를 가질 필요가 없어짐)

	GetAnimator()->CreateAnimation(L"JUMP", pTex, Vec2(0.f, 135.f), Vec2(36.f, 45.f), Vec2(36.f, 0.f), 0.1f, 8);
	
	CreateGravity();


	// 애니메이션에 오프셋을 주는 코드
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

	// 상태 업데이트
	update_state();

	update_attack();

	// 업데이트된 상태에 따라 애니메이션 업데이트
	update_animation();

	
	if (KEY_TAP(KEY::ENTER))
	{
		SetPos(Vec2(640.f, 320.f));
	}

	GetAnimator()->update();

	// 왜 첫줄 말고 마지막에 둬야할까...
	// 중간에 피격 상태가 되었다 가정
	// 이걸 첫 줄에 쓰면 prev와 cur이 같아져서 update_state 함수 예외처리에 걸려서 리턴돼버림
	m_ePrevState = m_eCurState;
	m_iPrevDir = m_iDir;
}
void CPlayer::render(HDC _dc)
{
	// 컴포넌트(충돌체, etc,,) 있는 경우 렌더
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
	//bf.SourceConstantAlpha = 127; //전역적으로 적용되는 알파!

	//// 목적지 dc, 출력위치(좌상단의 x, y), 가로세로, 원본 dc, 원본 이미지의 좌상단 xy, 너비높이, BLANDFUNCTION 구조체
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

	//미사일 객체
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
	// 키를 누르지 않아도 속도가 남아 있을 때를 고려한 조건수정
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
	{// 전 프레임과 같은 방향에서 눌렸을 때 또 가속도가 적용되면 안 됨
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
	// 상태와 방향이 모두 이전과 같다면 return
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
	// 바닥에 붙어 있는 상태 = 이미 중력 '상쇄' 중이므로 가속도를 따로 붙이지 않음
	// 그러다가 허공에 뜬 상태가 됐을 때, 아래로 향하는 가속도를 붙여준다
	
	if (!GetGravity()->GetGround())
	{
		GetRigidBody()->SetAccelAlpha(Vec2(0.f, 500.f));
	}

	// 점프를 해서 공중에 뜨는 그 '순간'부터 중력 적용됨
	// 점프란 공중을 향해 '힘'을 준 것...이지만
	// 게임적 허용으로, 점프 방향으로 바로 '속도'를 줌

	// 중력은 고정 가속도를 부여한다
	// 즉 일정한 가속도를 적용시킨다
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Ground")
	{
		Vec2 vPos = GetPos();

		// Ground보다 Player 좌표가 더 위다, 즉 Ground 위에 올라섰을 때
		if (vPos.y < pOtherObj->GetPos().y)
		{
			m_eCurState = PLAYER_STATE::IDLE;
		}
	}
}

