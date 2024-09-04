#include "pch.h"
#include "CScene_Start.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CGround.h"

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


CScene_Start::CScene_Start()
	: m_bUseForce(false)
	, m_fForceRadius(500.f)
	, m_fCurRadius(0.f)
	, m_fForce(500.f) // �Ÿ��� ���� ���� ���޽�ų ��
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::update()
{
	// Ŭ�� > �� ����
	if (KEY_HOLD(KEY::LBTN))
	{
		m_bUseForce = true;
		CreateForce();
	}
	else
	{
		m_bUseForce = false;
	}

	// Scene Update�� �� ������ ������ �޵��� ��
	// �θ� ���� update�ڵ� ��Ȱ�� ���� �ʰ� ��ü �����Ѵ�
	//CScene::update(); //�θ� �ڵ� ��Ȱ��
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
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

	// Scene ��ü
	//if (KEY_TAP(KEY::ENTER))
	//{
	//	ChangeScene(SCENE_TYPE::TOOL); // �̺�Ʈ ������
	//}



	//if (KEY_TAP(KEY::LBTN))
	//{
	//	// ���콺 ��ǥ�� �˾ƿͼ� �ű⸦ Lookat���� ����
	//	// ���콺 ��ǥ�� ������ �����̶�, �װ� ��¥��ǥ�� ��ȯ
	//	Vec2 vMousePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
	//	CCamera::GetInst()->SetLookAt(vMousePos);
	//}

	
}

void CScene_Start::render(HDC _dc)
{
	CScene::render(_dc);

	if (m_bUseForce)
	{
		SelectGDI gdi1(_dc, BRUSH_TYPE::HOLLOW);
		SelectGDI gdi2(_dc, PEN_TYPE::GREEN);

		m_fCurRadius += m_fForceRadius * 3.f * fDT;
		if (m_fCurRadius > m_fForceRadius)
		{
			m_fCurRadius = 0.f;
		}

		Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vForcePos);

		Ellipse(_dc
			, int(vRenderPos.x - m_fCurRadius)
			, int(vRenderPos.y - m_fCurRadius)
			, int(vRenderPos.x + m_fCurRadius)
			, int(vRenderPos.y + m_fCurRadius));
	}
	
}

void CScene_Start::Enter()
{
	// Object �߰�
	// �÷��̾� ������Ʈ�� �θ������ͷ� ����
	CObject* pObj = new CPlayer;
	pObj->SetName(L"Player");
	pObj->SetPos(Vec2(640.f, 320.f));
	pObj->SetScale(Vec2(100.f, 100.f));
	AddObject(pObj, GROUP_TYPE::PLAYER);

	RegisterPlayer(pObj);

	//CObject* pOtherPlayer = new CPlayer(*(CPlayer*)pObj);
	
	/*CObject* pOtherPlayer = pObj->Clone();
	pOtherPlayer->SetPos(Vec2(740.f, 360.f));
	AddObject(pOtherPlayer, GROUP_TYPE::PLAYER);*/

	// Follow Player
	//CCamera::GetInst()->SetTarget(pObj);


	Vec2 vResolution = CCore::GetInst()->GetResolution();
	// ���� ��ġ
	CMonster* pMon = CMonFactory::CreateMonster(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(0.f, 300));
	// AddObject�� CreateObject�� ����ó�� ������ ���� �Ǵ�
	// ��: Scene�� ��â ���ư��� ���� �����̶�� Create ���� �̺�Ʈó���� �ؾ���
	// CreateObject(pMon, GROUP_TYPE::MONSTER);
	AddObject(pMon, GROUP_TYPE::MONSTER);



	// �� ��ü ��ġ
	CGround* pGround = new CGround;
	pGround->SetPos(Vec2(640.f, 400.f));
	pGround->SetScale(Vec2(500.f, 100.f));
	pGround->SetName(L"Ground");
	AddObject(pGround, GROUP_TYPE::GROUND);



	//Ÿ�� �ε�
	//LoadTile(L"Tile\\start.tile");

	// �浹 ����
	// Player �׷�� Monster �׷� ���� �浹üũ
	// scene�� �������� �� �׷� ���� �浹 ���θ� �˻�
	// �浹 ������ ����...�� ������ִ� ���� (��ġ üũ�ڽ�ó��)
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER); // �÷��̾�-����
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::PROJ_PLAYER); // ����-�Ѿ�
	
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);

	// Camera Start ����
	// 	   ���� �ػ� �޾ƴ� ���� ��ġ���� �ϴ� �ڵ� �־ �ϴ� �ּ�ó��
	//Vec2 vResolution = CCore::GetInst()->GetResolution();
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);
	
	// Camera ȿ�� ����
	CCamera::GetInst()->FadeOut(1.f);
	CCamera::GetInst()->FadeIn(1.f);

	// Scene Enter ���̿� �� Start�� �־�����.
	start();
}

// �浹���� üũ�ڽ��� �����ϵ��� �浹 �׷� �������־�� ��
// ���� �������� �ٸ� ��ü������ �浹 �˻��� �� �����ϱ�
void CScene_Start::Exit()
{
	// ���� ��ü ����
	DeleteAll();

	// �׷� �浹 ���� ����
	CCollisionMgr::GetInst()->Reset();
}

void CScene_Start::CreateForce()
{
	m_vForcePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);

	// ��ü�� ������ ��� ��ü���� ���� ����
}


