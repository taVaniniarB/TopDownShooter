#include "pch.h"
#include "CMonster.h"
#include "AI.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "CTexture.h"
#include "CSceneMgr.h"
#include "CScene.h"

CMonster::CMonster()
	: m_tInfo{}
	, m_pAI(nullptr)
	, m_pWeapon(nullptr)
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(40.f, 40.f));
}

CMonster::~CMonster()
{// 몬스터 소멸 시 몬스터가 보유하고 있던 ai 소멸
	if (nullptr != m_pAI)
		delete m_pAI;
}


void CMonster::update()
{
	if (!GetEnabled())
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
}

// AI - 몬스터가 서로를 알도록 함
void CMonster::SetAI(AI* _AI)
{
	m_pAI = _AI;
	_AI->m_pOwner = this;
}

void CMonster::Save(FILE* _pFile)
{
	fwrite(&m_eType, sizeof(MON_TYPE), 1, _pFile);

	Vec2 vPos = GetPos();
	fwrite(&vPos, sizeof(Vec2), 1, _pFile);
}

void CMonster::Load(FILE* _pFile)
{
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	pScene->AddObject(this, GROUP_TYPE::MONSTER);
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	// 충돌한 오브젝트의 정체를 알아내자 (충돌체의 owner 반환하는 GetObj)
	CObject* pOtherObj = _pOther->GetObj();
	// obj 종류 알아내는 방법: 오브젝트에 아이디나 태그명 부여

	if (pOtherObj->GetName() == L"Player_Missile")
	{/*
		--m_iHP;

		if(m_iHP <= 0)
			DeleteObject(this);*/
	}
}