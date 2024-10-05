#include "pch.h"
#include "CScene_Combat.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CWall.h"
#include "CWeapon.h"
#include "CMousePtr.h"

#include "CCore.h"

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


CScene_Combat::CScene_Combat(const wstring& _wSceneRelativePath)
	: m_wSceneRelativePath( _wSceneRelativePath )
{
}

CScene_Combat::~CScene_Combat()
{
}

void CScene_Combat::update()
{
	CScene::update(); //�θ� �ڵ� ��Ȱ��

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
	
	CMousePtr* mousePtr = new CMousePtr();
	AddObject(mousePtr, GROUP_TYPE::MOUSE_POINTER);

	CObject* pPlayer = GetGroupObject(GROUP_TYPE::PLAYER)[0];
	RegisterPlayer(pPlayer);

	CWeapon* pWeapon = CWeaponFactory::CreateWeapon(WEAPON_TYPE::GUN, MELEE_TYPE::NONE, GUN_TYPE::M16);
	AddObject(pWeapon, GROUP_TYPE::WEAPON);
	((CPlayer*)pPlayer)->SetWeapon(pWeapon);

	CHitbox* pHitbox = new CHitbox();
	pHitbox->SetName(L"Hitbox_Player");
	AddObject(pHitbox, GROUP_TYPE::HITBOX_PLAYER);
	((CPlayer*)pPlayer)->SetHitbox(pHitbox);

	// Follow Player
	CCamera::GetInst()->SetTarget(pPlayer);

	
	// �浹 �׷� ����
	// scene�� �������� �� �׷� ���� �浹 ���θ� �˻��Ѵ�.
	// �浹 ������ ����...�� ������ִ� ���� (��ġ üũ�ڽ�ó��)
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
	
	
	// Camera Start ����
	//Vec2 vResolution = CCore::GetInst()->GetResolution();
	//CCamera::GetInst()->SetLookAt(vResolution / 2.f);
	
	// Camera ȿ�� ����
	CCamera::GetInst()->FadeIn(FADEIN_TIME);

	// Scene Enter ���̿� �� Start�� �־�����.
	start();
}

// �浹���� üũ�ڽ��� �����ϵ��� �浹 �׷� �������־�� ��
// ���� �������� �ٸ� ��ü������ �浹 �˻��� �� �����ϱ�
void CScene_Combat::Exit()
{
	// ���� ��ü ����
	DeleteAll();

	// �޺� �ʱ�ȭ
	//CScoreMgr::GetInst()->ResetCombo();

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


