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
	if (KEY_TAP(KEY::LBTN))
	{
		// ���콺 ��ǥ�� �˾ƿͼ� �ű⸦ Lookat���� ����
		// ���콺 ��ǥ�� ������ �����̶�, �װ� ��¥��ǥ�� ��ȯ
		Vec2 vMousePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
		CCamera::GetInst()->SetLookAt(vMousePos);
	}
}

void CScene_Start::Enter()
{
	// Object �߰�
	// �÷��̾� ������Ʈ�� �θ������ͷ� ����
	CObject* pObj = new CPlayer;

	pObj->SetPos(Vec2(640.f, 360.f));
	pObj->SetScale(Vec2(100.f, 100.f));

	AddObject(pObj, GROUP_TYPE::PLAYER);

	//CObject* pOtherPlayer = new CPlayer(*(CPlayer*)pObj);
	
	/*CObject* pOtherPlayer = pObj->Clone();
	pOtherPlayer->SetPos(Vec2(740.f, 360.f));
	AddObject(pOtherPlayer, GROUP_TYPE::PLAYER);*/

	// Follow Player
	//CCamera::GetInst()->SetTarget(pObj);


	// ���� �߰�
	int iMonsterCount = 2;
	float fMoveDist = 25.f;
	float fObjectScale = 50.f;

	// ���� ����� ���ÿ� �����ϸ� ���Կ����ڰ� �ƴ�, ��������� ���
	// > Vec2�� �����ڿ��� ó�� > ������ �����ε�
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	float fTerm = (vResolution.x - ((fMoveDist + fObjectScale/2.f) * 2)) / (float)(iMonsterCount - 1);
	CMonster* pMonsterObj = nullptr;

	for (int i = 0; i < iMonsterCount; ++i)
	{
		pMonsterObj = new CMonster;
		pMonsterObj->SetName(L"Monster");
		pMonsterObj->SetPos(Vec2((fMoveDist + fObjectScale / 2.f) + (float)i * fTerm, 50.f));
		pMonsterObj->SetScale(Vec2(fObjectScale, fObjectScale));
		pMonsterObj->SetMoveDist(fMoveDist);
		pMonsterObj->SetCenterPos(pMonsterObj->GetPos());
		AddObject(pMonsterObj, GROUP_TYPE::MONSTER);
	}

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


