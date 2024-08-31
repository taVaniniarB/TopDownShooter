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

#include "CRigidBody.h"

CPlayer::CPlayer()
{
	// 일단 컴포넌트는 부모의 private 멤버이다
	// 근데 컴포넌트 필요한 자식 쪽에서 직접 생성했으면 하니까
	// 자식쪽에서 부모에구현된 컴포넌트 생성 함수(Create)를 호출하여 컴포넌트를 생성하고
	// 컴포넌트의 기능을 이용할 땐 Getter 사용하여 해당 컴포넌트 주소를 받아온 다음
	// 포인터 역참조로 함수를 실행한다

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(Vec2(30.f, 55.f));
	
	CreateRigidBody();

	// Texture 로딩하기
	//CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\ray_t.bmp");
	CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\Fif.bmp");

	CreateAnimator();
	GetAnimator()->CreateAnimation(L"IDLE", pTex, Vec2(0.f, 0.f), Vec2(36.f, 40.f), Vec2(36.f, 0.f), 0.1f, 12);
	// Animator 안에 있는 IDLE이라는 Animation이 이 텍스처를 참조하고 이런 정보들을 참조했을 때
	// 원하는 장면을 출력하도록 한다 (따라서, 플레이어는 텍스처 멤버를 가질 필요가 없어짐)
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
	CRigidBody* pRigid = GetRigidBody();


	Vec2 vPos = GetPos();

	if (KEY_HOLD(KEY::W))
	{
		pRigid->AddForce(Vec2(0.f, -200.f));
	}
	if (KEY_HOLD(KEY::S))
	{
		pRigid->AddForce(Vec2(0.f, 200.f));
	}
	if (KEY_HOLD(KEY::A))
	{
		pRigid->AddForce(Vec2(-200.f, 0.f));
	}
	if (KEY_HOLD(KEY::D))
	{
		pRigid->AddForce(Vec2(200.f, 0.f));
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

