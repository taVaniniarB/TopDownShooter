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
	CScene::update(); //부모 코드 재활용

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL); // 이벤트 만들음
	}
	if (KEY_TAP(KEY::LBTN))
	{
		// 마우스 좌표를 알아와서 거기를 Lookat으로 잡자
		// 마우스 좌표는 렌더링 기준이라, 그걸 진짜좌표로 변환
		Vec2 vMousePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
		CCamera::GetInst()->SetLookAt(vMousePos);
	}
}

void CScene_Start::Enter()
{
	// Object 추가
	// 플레이어 오브젝트를 부모포인터로 저장
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


	// 몬스터 추가
	int iMonsterCount = 2;
	float fMoveDist = 25.f;
	float fObjectScale = 50.f;

	// 변수 선언과 동시에 대입하면 대입연산자가 아닌, 복사생성자 취급
	// > Vec2의 생성자에서 처리 > 생성자 오버로딩
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

	// 충돌 지정
	// Player 그룹과 Monster 그룹 간의 충돌체크
	// scene은 마지막에 두 그룹 간의 충돌 여부를 검사
	// 충돌 가능한 조합...을 만들어주는 개념 (마치 체크박스처럼)
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::PROJ_PLAYER);

	// Camera Start 지정
	// 	   위에 해상도 받아다 몬스터 위치지정 하는 코드 있어서 일단 주석처리
	//Vec2 vResolution = CCore::GetInst()->GetResolution();
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);
}

// 충돌조합 체크박스를 해제하듯이 충돌 그룹 해제해주어야 함
// 다음 씬에서는 다른 물체끼리의 충돌 검사할 수 있으니까
void CScene_Start::Exit()
{
	// 씬의 객체 삭제
	//DeleteAll();

	// 그룹 지정해서, 일부만 지우도록 하는 게 어떨까?

	DeleteAllException(GROUP_TYPE::PLAYER, GROUP_TYPE::PROJ_PLAYER);

	// 그룹 충돌 지정 해제
	CCollisionMgr::GetInst()->Reset();
}


