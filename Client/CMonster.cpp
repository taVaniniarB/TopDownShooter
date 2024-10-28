#include "pch.h"
#include "CMonster.h"
#include "AI.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "CTexture.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CWeapon.h"
#include "CHitbox.h"

#include "SelectGDI.h"
#include "CSoundMgr.h"

CMonster::CMonster()
	: m_tInfo{}
	, m_pAI(nullptr)
	, m_pWeapon(nullptr)
	, m_pHitbox(nullptr)
	, m_eType(MON_TYPE::NORMAL)
{
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 3.f));
	GetCollider()->SetScale(Vec2(15.f, 15.f));
}

CMonster::~CMonster()
{// 몬스터 소멸 시 몬스터가 보유하고 있던 ai 소멸
	if (nullptr != m_pAI)
		delete m_pAI;
}


void CMonster::update()
{
	if (CSceneMgr::GetInst()->GetCurScene()->GetName() == L"Tool Scene")
		return;

	if (nullptr != m_pAI)
	{
		m_pAI->update();
	}
}

void CMonster::render(HDC _dc)
{
	Vec2 vPos = CCamera::GetInst()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();
	CTexture* pTex = GetTexture();

	TransparentBlt(_dc
		, (int)(vPos.x - vScale.x / 2.f)
		, (int)(vPos.y - vScale.y / 2.f)
		, (int)(vScale.x)
		, (int)(vScale.y)
		, pTex->GetDC()
		, (int)(0) // 이미지 상 좌상단좌표 x,y
		, (int)(0)
		, (int)(vScale.x)
		, (int)(vScale.y)
		, RGB(255, 0, 255));

#ifdef _DEBUG
	//renderRay(_dc);
#endif
}


void CMonster::renderRay(HDC _dc)
{
	PEN_TYPE ePen = PEN_TYPE::GREEN;
	if (m_pAI->GetCurState() == MON_STATE::TRACE)
		ePen = PEN_TYPE::RED;

	SelectGDI p(_dc, ePen);

	Vec2 vPos = CCamera::GetInst()->GetRenderPos(GetPos());
	Vec2 vPlayerPos = CSceneMgr::GetInst()->GetCurScene()->GetPlayer()->GetPos();
	vPlayerPos = CCamera::GetInst()->GetRenderPos(vPlayerPos);

	MoveToEx(_dc, vPos.x, vPos.y, NULL);
	LineTo(_dc, vPlayerPos.x, vPlayerPos.y);
}

// AI - 몬스터가 서로를 알도록 함
void CMonster::SetAI(AI* _AI)
{
	m_pAI = _AI;
	_AI->m_pOwner = this;
}

void CMonster::SetWeapon(CWeapon* _pWeapon)
{
	if (m_pWeapon)
		assert(nullptr); // 이미 무기 존재

	m_pWeapon = _pWeapon;
	_pWeapon->SetOwner(this);
}

void CMonster::SetHitbox(CHitbox* _pHitbox)
{
	m_pHitbox = _pHitbox;
	_pHitbox->SetOwner(this);
}

void CMonster::DropWeapon()
{
	if (m_pWeapon)
	{
		CObject* pNewWeapon = m_pWeapon->Clone();
		((CWeapon*)pNewWeapon)->Drop();
		CreateObject(pNewWeapon, GROUP_TYPE::DROPPED_WEAPON);
		DeleteObject(m_pWeapon);
		m_pWeapon = nullptr;
	}
}

void CMonster::SubHP(wstring _strWeapon)
{
	--m_tInfo.iHP;

	if (m_tInfo.iHP <= 0)
	{
		MonsterDeath(_strWeapon);
	}
}

void CMonster::MonsterDeath(std::wstring& _strWeapon)
{
	if (_strWeapon == L"Knife")
		CSoundMgr::GetInst()->PlaySE(L"Cut1", 30.f);
	else if (_strWeapon == L"Pipe")
		CSoundMgr::GetInst()->PlaySE(L"Hit1", 30.f);

	// 피 튀는 파티클
	DropWeapon();
	DeleteObject(this);
	DeleteObject(GetHitbox());

	CSceneMgr::GetInst()->GetCurScene()->CreateBlood(GetPos());
}

void CMonster::Save(FILE* _pFile)
{
	fwrite(&m_eType, sizeof(MON_TYPE), 1, _pFile);

	Vec2 vPos = GetPos();
	fwrite(&vPos, sizeof(Vec2), 1, _pFile);

	FULL_WEAPON_TYPE fwt = GetWeapon()->GetFullWeaponType();
	fwrite(&fwt, sizeof(fwt), 1, _pFile);
}

void CMonster::Load(FILE* _pFile)
{
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	pScene->AddObject(this, GROUP_TYPE::MONSTER);
	if (nullptr != m_pWeapon)
	{
		pScene->AddObject(m_pWeapon, GROUP_TYPE::WEAPON);
	}
	if (nullptr != m_pHitbox)
	{
		pScene->AddObject(m_pHitbox, GROUP_TYPE::HITBOX_MONSTER);
	}
}


void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	// 충돌한 오브젝트의 정체를 알아내자 (충돌체의 owner 반환하는 GetObj)
	CObject* pOtherObj = _pOther->GetObj();
	// obj 종류 알아내는 방법: 오브젝트에 아이디나 태그명 부여

	if (pOtherObj->GetName() == L"Missile")
	{
		//--m_tInfo.iHP;

		//if (m_tInfo.iHP <= 0)
		//{
		//	// 피 튀는 파티클
		//	DropWeapon();
		//	DeleteObject(this);

		//	// 시체 + 피 스프라이트
		//}
	}
}