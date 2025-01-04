#include "pch.h"
#include "CPlayer.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CResMgr.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#include "CRigidBody.h"
#include "CWeapon.h"
#include "CHitbox.h"
#include "CUI.h"
#include "CGun.h"
#include "CSoundMgr.h"

CPlayer::CPlayer()
	: m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::WALK)
	, m_iDir(1)
	, m_fSpeed(250.f)
	, m_iPrevDir(1)
	, m_pHitbox(nullptr)
	, m_pWeapon(nullptr)
	, m_iHP(1)
{
	// 일단 컴포넌트는 부모의 private 멤버이다
	// 근데 컴포넌트 필요한 자식 쪽에서 직접 생성했으면 하니까
	// 자식쪽에서 부모에구현된 컴포넌트 생성 함수(Create)를 호출하여 컴포넌트를 생성하고
	// 컴포넌트의 기능을 이용할 땐 Getter 사용하여 해당 컴포넌트 주소를 받아온 다음
	// 포인터 역참조로 함수를 실행한다
	SetName(L"Player");
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 3.f));
	GetCollider()->SetScale(Vec2(15.f, 15.f));
	
	

	CreateRigidBody();

	// Texture 로딩하기
	// CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\ray_t.bmp");
	//CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\Fif.bmp");


	CreateAnimator();
	
	
	GetAnimator()->LoadAnimation(L"animation\\player_idle_left.anim");
	GetAnimator()->LoadAnimation(L"animation\\player_idle_right.anim");
	GetAnimator()->LoadAnimation(L"animation\\player_run.anim");

	if (CSceneMgr::GetInst()->GetCurScene()->GetName() == L"Tool Scene")
		SetEnabled(false);
	
	//
	//// Animator 안에 있는 IDLE이라는 Animation이 이 텍스처를 참조하고 이런 정보들을 참조했을 때
	//// 원하는 장면을 출력하도록 한다 (따라서, 플레이어는 텍스처 멤버를 가질 필요가 없어짐)
	//GetAnimator()->CreateAnimation(L"IDLE_RIGHT", pTex, Vec2(0.f, 0.f), Vec2(36.f, 40.f), Vec2(36.f, 0.f), 0.1f, 12);
	//GetAnimator()->CreateAnimation(L"IDLE_LEFT", pTex, Vec2(0.f, 0.f), Vec2(36.f, 40.f), Vec2(36.f, 0.f), 0.1f, 12);
	//GetAnimator()->CreateAnimation(L"RUN", pTex, Vec2(0.f, 52.f), Vec2(46.f, 38.f), Vec2(46.f, 0.f), 0.1f, 10);
	//GetAnimator()->CreateAnimation(L"JUMP", pTex, Vec2(0.f, 135.f), Vec2(36.f, 45.f), Vec2(36.f, 0.f), 0.1f, 8);
	//
	//
	//// Animation 저장해보기
	//GetAnimator()->FindAnimation(L"IDLE_LEFT")->Save(L"animation\\player_idle_left.anim");
	//GetAnimator()->FindAnimation(L"IDLE_RIGHT")->Save(L"animation\\player_idle_right.anim");
	//GetAnimator()->FindAnimation(L"RUN")->Save(L"animation\\player_run.anim");
	//GetAnimator()->FindAnimation(L"JUMP")->Save(L"animation\\player_jump.anim");
	//


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
	if (GetEnabled())
		update_move();

	// 상태 업데이트
	update_state();

	update_attack();

	// 업데이트된 상태에 따라 애니메이션 업데이트
	update_animation();

	if (KEY_TAP(KEY::RBTN))
	{
		DropWeapon();
	}

	SetAimDir();
	
	GetAnimator()->update();

	// 왜 첫줄 말고 마지막에 둬야할까...
	// 중간에 피격 상태가 되었다 가정
	// 이걸 첫 줄에 쓰면 prev와 cur이 같아져서 update_state 함수 예외처리에 걸려서 리턴돼버림
	m_ePrevState = m_eCurState;
	m_iPrevDir = m_iDir;

}
void CPlayer::render(HDC _dc)
{
	component_render(_dc);
}



void CPlayer::update_state()
{
	if (KEY_HOLD(KEY::A))
	{
		m_iDir = -1;
		m_eCurState = PLAYER_STATE::RUN;
			
	}
	if (KEY_HOLD(KEY::D))
	{
		m_iDir = 1;
		m_eCurState = PLAYER_STATE::RUN;
	}
	if (KEY_TAP(KEY::W))
	{
		m_eCurState = PLAYER_STATE::RUN;
		
	}
	if (KEY_TAP(KEY::S))
	{
		m_eCurState = PLAYER_STATE::RUN;
	}

	if (KEY_AWAY(KEY::A) || KEY_AWAY(KEY::D) || KEY_AWAY(KEY::W) || KEY_AWAY(KEY::S))
	{
		m_eCurState = PLAYER_STATE::IDLE;
	}
}

void CPlayer::update_move()
{
	Vec2 vPlayerMoveVec(0.f, 0.f);

	Vec2 vPos = GetPos();

	if (KEY_HOLD(KEY::W))
	{
		vPlayerMoveVec += Vec2(0.f, -1.f);
	}
	if (KEY_HOLD(KEY::S))
	{
		vPlayerMoveVec += Vec2(0.f, 1.f);
	}
	if (KEY_HOLD(KEY::A))
	{
		vPlayerMoveVec += Vec2(-1.f, 0.f);
	}
	if (KEY_HOLD(KEY::D))
	{
		vPlayerMoveVec += Vec2(1.f, 0.f);
	}


	if (!vPlayerMoveVec.isZero())
	{
		vPlayerMoveVec.Normalize();
		vPos += vPlayerMoveVec * m_fSpeed * fDT;
		SetPos(vPos);
	}
}

void CPlayer::update_attack()
{
	if (!m_pWeapon)
		return;

	WEAPON_TYPE wt = m_pWeapon->GetWeaponType();

	if (wt == WEAPON_TYPE::MELEE)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			if (m_pWeapon)
			{
				m_pWeapon->Attack();
			}
		}
	}
	else if (wt == WEAPON_TYPE::GUN)
	{
		if (KEY_HOLD(KEY::LBTN))
		{
			if (m_pWeapon)
			{
				m_pWeapon->Attack();
			}
		}
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
	case PLAYER_STATE::ATTACK:
		break;
	case PLAYER_STATE::DEAD:
		break;
	default:
		break;
	}
}

void CPlayer::DropWeapon()
{
	if (m_pWeapon)
	{
		if (m_pWeapon->GetWeaponType() == WEAPON_TYPE::GUN)
		{
			CSceneMgr::GetInst()->GetCurScene()->SetUIVisable(L"ammoImage", false);
			CSceneMgr::GetInst()->GetCurScene()->SetUIVisable(L"ammoUI", false);
		}

		CObject* pNewWeapon = m_pWeapon->Clone();
		((CWeapon*)pNewWeapon)->Drop();
		CreateObject(pNewWeapon, GROUP_TYPE::DROPPED_WEAPON);
		DeleteObject(m_pWeapon);
		m_pWeapon = nullptr;
	}
}

void CPlayer::SetWeapon(CWeapon* _pWeapon)
{
	m_pWeapon = _pWeapon;
	_pWeapon->SetOwner(this);
}

void CPlayer::SetHitbox(CHitbox* _pHitbox)
{
	m_pHitbox = _pHitbox;
	_pHitbox->SetOwner(this);
}

void CPlayer::SubHP(wstring _strWeapon)
{
	if (m_iHP == 0)
		return;

	--m_iHP;
	CSceneMgr::GetInst()->GetCurScene()->SetHPUI(m_iHP);

	if (m_iHP <= 0)
	{
		PlayerDeath(_strWeapon);
	}
}

void CPlayer::PlayerDeath(std::wstring& _strWeapon)
{
	if (_strWeapon == L"Knife")
		CSoundMgr::GetInst()->PlaySE(L"Cut1", 30.f);
	else if (_strWeapon == L"Pipe")
		CSoundMgr::GetInst()->PlaySE(L"Hit1", 30.f);
	DropWeapon();

	DeleteObject(this);

	CSceneMgr::GetInst()->GetCurScene()->CreateBlood(GetPos());
	CSceneMgr::GetInst()->GetCurScene()->PlayerDeath();
}

void CPlayer::PickupWeapon(CWeapon* _pWeapon)
{
	// 떨어진 무기를 주운 상황일 경우
	if (_pWeapon->GetStatus() == WEAPON_STATUS::DROPPED)
	{
		CObject* pNewWeapon = _pWeapon->Clone();
		DeleteObject(_pWeapon);
		CreateObject(pNewWeapon, GROUP_TYPE::WEAPON);
		((CWeapon*)pNewWeapon)->SetStatus(WEAPON_STATUS::HOLD);
		SetWeapon((CWeapon*)pNewWeapon);

		if (((CWeapon*)pNewWeapon)->GetWeaponType() == WEAPON_TYPE::GUN)
		{
			CSceneMgr::GetInst()->GetCurScene()->SetUIVisable(L"ammoImage", true);
			CSceneMgr::GetInst()->GetCurScene()->SetUIVisable(L"ammoUI", true);
			CSceneMgr::GetInst()->GetCurScene()->SetUIText(L"ammoUI", ((CGun*)pNewWeapon)->GetAmmoNum());
			
		}
		else
		{
			CSceneMgr::GetInst()->GetCurScene()->SetUIVisable(L"ammoUI", false);
			CSceneMgr::GetInst()->GetCurScene()->SetUIVisable(L"ammoImage", false);
		}
	}
}

void CPlayer::SetAimDir()
{
	if (m_pWeapon)
	{
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);

		Vec2 vPos = GetPos();

		m_vAimDir = vMousePos - vPos;
		m_vAimDir.Normalize();
		
		m_pWeapon->SetAimDir(m_vAimDir);
	}
}

// 설정된 무기가 있는 상태에서만 실행
void CPlayer::ExchangeWeapon(CWeapon* _pWeapon)
{
	DropWeapon();
	PickupWeapon(_pWeapon);
}


void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	/*CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Missile")
	{
		--m_iHP;
	}*/
}

void CPlayer::Save(FILE* _pFile)
{
	Vec2 vPos = GetPos();
	fwrite(&vPos, sizeof(Vec2), 1, _pFile);
}

void CPlayer::Load(FILE* _pFile)
{
	Vec2 vPos = {};
	fread(&vPos, sizeof(Vec2), 1, _pFile);
	SetPos(vPos);

	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	pScene->AddObject(this, GROUP_TYPE::PLAYER);
}

void CPlayer::OnCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Weapon")
	{
		CWeapon* pWeapon = (CWeapon*)pOtherObj;
		// 무기이자 드롭된무기이자 우클릭상태일 때
		if (pWeapon->GetStatus() == WEAPON_STATUS::DROPPED
			&& KEY_TAP(KEY::RBTN))
		{
			if (m_pWeapon)
				ExchangeWeapon(pWeapon);
			else
				PickupWeapon(pWeapon);
		}
	}
}