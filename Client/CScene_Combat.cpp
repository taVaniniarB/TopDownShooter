#include "pch.h"
#include "CScene_Combat.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CWall.h"
#include "CWeapon.h"
#include "CMousePtr.h"

#include "CCore.h"
#include "CStage.h"
#include "CPathMgr.h"
#include "CTexture.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CCamera.h"
#include "AI.h"
#include "CIdleState.h"
#include "CTraceState.h"
#include "CRigidBody.h"
#include "SelectGDI.h"
#include "CTimeMgr.h"
#include "CScoreMgr.h"
#include "CHitbox.h"
#include "CGun.h"
#include "CAmmoImgUI.h"
#include "CAmmoUI.h"
#include "CScoreUI.h"
#include "CComboUI.h"
#include "CHPUI.h"
#include "CRestartUI.h"


CScene_Combat::CScene_Combat(const wstring& _wSceneRelativePath)
	: m_wSceneRelativePath( _wSceneRelativePath )
	, m_iCombo(0)
	, m_fCurTime(0)
	, m_fComboTime(2.f)
{
}

CScene_Combat::~CScene_Combat()
{
}

void CScene_Combat::update()
{
	CScene::update(); //�θ� �ڵ� ��Ȱ��

	if (!GetIsPlayerAlive())
	{
		if (GetPlayerPrevAlive())
		{
			CUI* pRestartUI = new CRestartUI;
			pRestartUI->SetName(L"RestartUI");
			pRestartUI->SetVisable(true);
			AddObject(pRestartUI, GROUP_TYPE::UI);

			SetPlayerPrevAlive(false);
		}

		if (KEY_TAP(KEY::R))
		{
			CStage::GetInst()->SetSavedScore();
			ChangeScene(GetSceneType());
		}
	}


	const vector<CObject*>& vecMonster = CSceneMgr::GetInst()->GetCurScene()->GetGroupObject(GROUP_TYPE::MONSTER);
	for (size_t i = 0; vecMonster.size() > i; ++i)
	{
		// ���� ���� > DeleteObject ���ϱ� ���� ���� ����
		if (vecMonster[i]->IsDead())
		{
			AddScore();
		}
	}

	m_fCurTime += fDT;

	if (m_fCurTime > m_fComboTime)
	{
		ResetCombo();
		SetUIVisable(L"ComboUI", false);
	}
	/*for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		const vector<CObject*>& vecObj = GetGroupObject((GROUP_TYPE)i);

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (!vecObj[j]->IsDead())
			{
				// Scene�� ���� �߻��߰�, �ش� ��ü�� ��ü�� �����ϰ� ���� ���
				if (m_bUseForce &&
					vecObj[j]->GetRigidBody())
				{
					// ��ü�� ��ġ - ���� ��ġ (�� ��ġ>��ü ��ġ ���ϴ� ����) 
					// �� �Ÿ��� ª�� ���� ū ���� �޵���
					Vec2 vDiff = vecObj[j]->GetPos() - m_vForcePos;
					float fLen = vDiff.Length();
					if (fLen < m_fForceRadius)
					{
						float fRatio = 1.f - (fLen / m_fForceRadius);
						
						// �ִ� ���� �Ÿ� ���� ����
						float fForce = m_fForce * fRatio;

						vecObj[j]->GetRigidBody()->AddForce(vDiff.Nomalize() * fForce);
					}
				}
				vecObj[j]->update();
			}
		}
	}
	*/

	//if (KEY_TAP(KEY::LBTN))
	//{
	//	// ���콺 ��ǥ�� �˾ƿͼ� �ű⸦ Lookat���� ����
	//	// ���콺 ��ǥ�� ������ �����̶�, �װ� ��¥��ǥ�� ��ȯ
	//	Vec2 vMousePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
	//	CCamera::GetInst()->SetLookAt(vMousePos);
	//}
}


// ������ HP�� 0�� ���� �� ȣ��
void CScene_Combat::AddScore()
{
	AddCombo();

	// �� ó�� �� ����
	// �⺻ 100�� + ���� �޺��� 50�����ŭ �����Ѵ�.
	CStage::GetInst()->AddScore(100 + (m_iCombo * 50));

}

void CScene_Combat::AddCombo()
{
	++m_iCombo;
	m_fCurTime = 0;

	SetUIVisable(L"ComboUI", true);
	SetUIText(L"ComboUI", m_iCombo);
}
void CScene_Combat::CreateCombatSceneUI()
{
	// ����, �� ����(��� �ִ� ���⿡ ����/���� drop�� pickup �� ��ü/���� ���Ⱑ ���� ���� ǥ��)
	// �÷��̾� HP, �޺�(�޺�Ÿ�� ������ ǥ��)
	CUI* pScoreUI = new CScoreUI;
	pScoreUI->SetName(L"ScoreUI");
	AddObject(pScoreUI, GROUP_TYPE::UI);
	
	CUI* pComboUI = new CComboUI;
	pComboUI->SetName(L"ComboUI");
	AddObject(pComboUI, GROUP_TYPE::UI);

	CUI* pRemainAmmoUI = new CAmmoUI;
	pRemainAmmoUI->SetName(L"ammoUI");
	pRemainAmmoUI->SetVisable(false);
	AddObject(pRemainAmmoUI, GROUP_TYPE::UI);
	
	CUI* pAmmoImgUI = new CAmmoImgUI;
	pAmmoImgUI->SetName(L"ammoImage");
	pAmmoImgUI->SetVisable(false);
	AddObject(pAmmoImgUI, GROUP_TYPE::UI);

	CUI* pHPUI = new CHPUI;
	pHPUI->SetName(L"HPUI");
	pHPUI->SetVisable(true);
	AddObject(pHPUI, GROUP_TYPE::UI);
}

void CScene_Combat::render(HDC _dc)
{
	CScene::render(_dc);

	//if (m_bUseForce)
	//{
	//	SelectGDI gdi1(_dc, BRUSH_TYPE::HOLLOW);
	//	SelectGDI gdi2(_dc, PEN_TYPE::GREEN);
	//	m_fCurRadius += m_fForceRadius * 3.f * fDT;
	//	if (m_fCurRadius > m_fForceRadius)
	//	{
	//		m_fCurRadius = 0.f;
	//	}
	//	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vForcePos);
	//	Ellipse(_dc
	//		, int(vRenderPos.x - m_fCurRadius)
	//		, int(vRenderPos.y - m_fCurRadius)
	//		, int(vRenderPos.x + m_fCurRadius)
	//		, int(vRenderPos.y + m_fCurRadius));
	//}
	//
}

void CScene_Combat::Enter()
{
	LoadScene(m_wSceneRelativePath);

	CreateCombatSceneUI();
	CStage::GetInst()->SetSavedScore();

	CMousePtr* mousePtr = new CMousePtr();
	AddObject(mousePtr, GROUP_TYPE::MOUSE_POINTER);

	CObject* pPlayer = GetGroupObject(GROUP_TYPE::PLAYER)[0];

	PlayerSetting(pPlayer);

	// �ٴڿ� ���� �ӽ� ����
	//CWeapon* ptestWeapon = CWeaponFactory::CreateWeapon(FULL_WEAPON_TYPE::M16);
	//ptestWeapon->Drop();
	//AddObject(ptestWeapon, GROUP_TYPE::DROPPED_WEAPON);
	//ptestWeapon->SetPos(pPlayer->GetPos());

	// �浹 �׷� ����
	CreateCollisionGroup();
	
	CameraSetting(pPlayer);

	// Scene Enter ���̿� �� Start�� �־�����.
	start();
}

void CScene_Combat::PlayerSetting(CObject* pPlayer)
{
	int playerInitHP = 10;

	RegisterPlayer(pPlayer);
	SetPlayerAlive(true);


	CHitbox* pHitbox = new CHitbox(Vec2(15.f, 25.f));
	pHitbox->SetName(L"Hitbox_Player");
	AddObject(pHitbox, GROUP_TYPE::HITBOX_PLAYER);
	((CPlayer*)pPlayer)->SetHitbox(pHitbox);


	((CPlayer*)pPlayer)->SetHP(playerInitHP);
	SetHPUI(playerInitHP);


	CWeapon* pWeapon = CStage::GetInst()->GetPlayerWeapon();
	if (nullptr != pWeapon)
	{
		CObject* pNewWeapon = pWeapon->Clone();
		AddObject(pNewWeapon, GROUP_TYPE::WEAPON);
		((CPlayer*)pPlayer)->SetWeapon((CWeapon*)pNewWeapon);

		if (pWeapon->GetWeaponType() == WEAPON_TYPE::GUN)
		{
			SetUIText(L"ammoUI", ((CGun*)pWeapon)->GetAmmoNum());
			SetUIVisable(L"ammoImage", true);
			SetUIVisable(L"ammoUI", true);
		}
	}
}

void CScene_Combat::CameraSetting(CObject* pPlayer)
{
	// Camera Start ����
	//Vec2 vResolution = CCore::GetInst()->GetResolution();
	//CCamera::GetInst()->SetLookAt(vResolution / 2.f);
	// Follow Player
	CCamera::GetInst()->SetTarget(pPlayer);
	// Camera ȿ�� ����
	CCamera::GetInst()->FadeIn(FADEIN_TIME);
}

void CScene_Combat::CreateCollisionGroup()
{
	// scene�� �������� �� �׷� ���� �浹 ���θ� �˻��Ѵ�.
	// �浹 ������ ������ ������ִ� ���� (üũ�ڽ�ó��)

	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::DROPPED_WEAPON); // ����������-�÷��̾�

	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_PLAYER, GROUP_TYPE::WALL); // ��-�÷��̾� �Ѿ�
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_PLAYER, GROUP_TYPE::TILE_WALL);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_PLAYER, GROUP_TYPE::CORNER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_MONSTER, GROUP_TYPE::WALL); // ��-���� �Ѿ�
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_MONSTER, GROUP_TYPE::TILE_WALL);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_MONSTER, GROUP_TYPE::CORNER);

	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::WALL); // �÷��̾�-��
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::TILE_WALL);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::CORNER);

	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::WALL); // ����-��
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::TILE_WALL);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::CORNER);

	// ��Ʈ�ڽ� - �Ѿ�
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::HITBOX_PLAYER, GROUP_TYPE::PROJ_MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::HITBOX_MONSTER, GROUP_TYPE::PROJ_PLAYER);

	// �÷��̾� - SC
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::SCENE_CHANGER);
}

// �浹���� üũ�ڽ��� �����ϵ��� �浹 �׷� �������־�� ��
// ���� �������� �ٸ� ��ü������ �浹 �˻��� �� �����ϱ�
void CScene_Combat::Exit()
{
	// �÷��̾� ���� ����
	if (GetIsPlayerAlive())
	{
		CObject* pPlayer = GetPlayer();
		CStage::GetInst()->SavePlayerWeapon(((CPlayer*)pPlayer)->GetWeapon());

	}

		
	
	// ���� ��ü ����
	DeleteAll();
	// �׷� �浹 ���� ����
	CCollisionMgr::GetInst()->Reset();
}
//
//void CScene_Start::CreateForce()
//{
//	m_vForcePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
//
//	// ��ü�� ������ ��� ��ü���� ���� ����
//}


