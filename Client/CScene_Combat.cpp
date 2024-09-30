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

	// Scene ��ü
	// ���߿� SceneChanger - OncollisionEnter�� �Ű�����
	if (KEY_TAP(KEY::ENTER))
	{
		CCamera::GetInst()->FadeOut(FADEOUT_TIME);
		CScene::SetEnabled(false);
		CSceneMgr::GetInst()->SetSceneChange(true, SCENE_TYPE::TOOL);
	}


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
	 
	// Object �߰�
	// �÷��̾� ������Ʈ�� �θ������ͷ� ����
	CObject* pObj = new CPlayer;
	pObj->SetName(L"Player");
	pObj->SetPos(Vec2(640.f, 500.f));
	pObj->SetScale(Vec2(100.f, 100.f));
	AddObject(pObj, GROUP_TYPE::PLAYER);

	RegisterPlayer(pObj);

	// Follow Player
	CCamera::GetInst()->SetTarget(pObj);


	Vec2 vResolution = CCore::GetInst()->GetResolution();
	// ���� ��ġ

	CMonster* pMon = CMonFactory::CreateMonster(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(0.f, 300));
	// AddObject�� CreateObject�� ����ó�� ������ ���� �Ǵ�
	// ��: Scene�� ��â ���ư��� ���� �����̶�� Create ���� �̺�Ʈó���� �ؾ���
	//CreateObject(pMon, GROUP_TYPE::MONSTER);
	AddObject(pMon, GROUP_TYPE::MONSTER);

	WEAPON_TYPE eWeaponType = WEAPON_TYPE::GUN;
	CWeapon* pWeapon = CWeaponFactory::CreateWeapon(eWeaponType, MELEE_TYPE::NONE, GUN_TYPE::M16);
	pWeapon->SetScale(Vec2(10.f, 20.f));
	AddObject(pWeapon, GROUP_TYPE::WEAPON);
	
	// �׽�Ʈ������ public���� Ǯ������� ���߿� �ٽ� private���� �����صѰ�
	((CPlayer*)pObj)->SetWeapon(pWeapon);
	
	// �浹 ����
	// Player �׷�� Monster �׷� ���� �浹üũ
	// scene�� �������� �� �׷� ���� �浹 ���θ� �˻�
	// �浹 ������ ����...�� ������ִ� ���� (��ġ üũ�ڽ�ó��)
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::PROJ_PLAYER); // ����-pl�Ѿ�
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::DROPPED_WEAPON); // ����������-�÷��̾�
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_PLAYER, GROUP_TYPE::WALL); // ��-�÷��̾� �Ѿ�
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_PLAYER, GROUP_TYPE::TILE_WALL); // ��-�÷��̾� �Ѿ�
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_PLAYER, GROUP_TYPE::CORNER); // ��-�÷��̾� �Ѿ�
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::WALL);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::TILE_WALL);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::CORNER);
	// Camera Start ����
	// ���� �ػ� �޾ƴ� ���� ��ġ���� �ϴ� �ڵ� �־ �ϴ� �ּ�ó��
	//Vec2 vResolution = CCore::GetInst()->GetResolution();
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);
	
	// Camera ȿ�� ����
	CCamera::GetInst()->FadeIn(FADEIN_TIME);

	// Scene Enter ���̿� �� Start�� �־�����.
	start();
}

// �浹���� üũ�ڽ��� �����ϵ��� �浹 �׷� �������־�� ��
// ���� �������� �ٸ� ��ü������ �浹 �˻��� �� �����ϱ�
void CScene_Combat::Exit()
{
	// invoke(�Լ�, �ð�)
	// �ð� ���� �Լ��� ����?
	// FadeOut ���� �ð� ���� ���� Scene Enter�� ������� �ʵ��� �ϱ�?
 
	// ���� ��ü ����
	DeleteAll();

	// �޺� �ʱ�ȭ
	CScoreMgr::GetInst()->ResetCombo();

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


