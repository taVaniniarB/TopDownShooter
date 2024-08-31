#include "pch.h"
#include "CScene_Start.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"

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

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::update()
{
	CScene::update(); //�θ� �ڵ� ��Ȱ��

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL); // �̺�Ʈ ������
	}
	//if (KEY_TAP(KEY::LBTN))
	//{
	//	// ���콺 ��ǥ�� �˾ƿͼ� �ű⸦ Lookat���� ����
	//	// ���콺 ��ǥ�� ������ �����̶�, �װ� ��¥��ǥ�� ��ȯ
	//	Vec2 vMousePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
	//	CCamera::GetInst()->SetLookAt(vMousePos);
	//}
}

void CScene_Start::Enter()
{
	// Object �߰�
	// �÷��̾� ������Ʈ�� �θ������ͷ� ����
	CObject* pObj = new CPlayer;
	pObj->SetPos(Vec2(640.f, 360.f));
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

	//Ÿ�� �ε�
	//LoadTile(L"Tile\\start.tile");

	// �浹 ����
	// Player �׷�� Monster �׷� ���� �浹üũ
	// scene�� �������� �� �׷� ���� �浹 ���θ� �˻�
	// �浹 ������ ����...�� ������ִ� ���� (��ġ üũ�ڽ�ó��)
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::PROJ_PLAYER);

	// Camera Start ����
	// 	   ���� �ػ� �޾ƴ� ���� ��ġ���� �ϴ� �ڵ� �־ �ϴ� �ּ�ó��
	//Vec2 vResolution = CCore::GetInst()->GetResolution();
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);
	
	// Camera ȿ�� ����
	CCamera::GetInst()->FadeOut(1.f);
	CCamera::GetInst()->FadeIn(1.f);
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


