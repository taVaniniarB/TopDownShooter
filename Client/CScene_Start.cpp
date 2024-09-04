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
	, m_fForce(500.f) // 거리에 따라 적게 전달시킬 것
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::update()
{
	// 클릭 > 힘 생성
	if (KEY_HOLD(KEY::LBTN))
	{
		m_bUseForce = true;
		CreateForce();
	}
	else
	{
		m_bUseForce = false;
	}

	// Scene Update가 힘 생성의 영향을 받도록 함
	// 부모 쪽의 update코드 재활용 하지 않고 자체 구현한다
	//CScene::update(); //부모 코드 재활용
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		const vector<CObject*>& vecObj = GetGroupObject((GROUP_TYPE)i);

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (!vecObj[j]->IsDead())
			{
				// Scene에 힘이 발생했고, 해당 물체가 강체를 보유하고 있을 경우
				if (m_bUseForce &&
					vecObj[j]->GetRigidBody())
				{
					// 물체의 위치 - 힘의 위치 (힘 위치>물체 위치 향하는 벡터) 
					// 이 거리가 짧을 수록 큰 힘을 받도록
					Vec2 vDiff = vecObj[j]->GetPos() - m_vForcePos;
					float fLen = vDiff.Length();
					if (fLen < m_fForceRadius)
					{
						float fRatio = 1.f - (fLen / m_fForceRadius);
						
						// 최대 힘에 거리 비율 적용
						float fForce = m_fForce * fRatio;

						vecObj[j]->GetRigidBody()->AddForce(vDiff.Nomalize() * fForce);
					}
				}
				vecObj[j]->update();
			}
		}
	}

	// Scene 교체
	//if (KEY_TAP(KEY::ENTER))
	//{
	//	ChangeScene(SCENE_TYPE::TOOL); // 이벤트 만들음
	//}



	//if (KEY_TAP(KEY::LBTN))
	//{
	//	// 마우스 좌표를 알아와서 거기를 Lookat으로 잡자
	//	// 마우스 좌표는 렌더링 기준이라, 그걸 진짜좌표로 변환
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
	// Object 추가
	// 플레이어 오브젝트를 부모포인터로 저장
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
	// 몬스터 배치
	CMonster* pMon = CMonFactory::CreateMonster(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(0.f, 300));
	// AddObject와 CreateObject는 지연처리 유무에 따라 판단
	// 예: Scene이 한창 돌아가는 도중 생성이라면 Create 통한 이벤트처리로 해아함
	// CreateObject(pMon, GROUP_TYPE::MONSTER);
	AddObject(pMon, GROUP_TYPE::MONSTER);



	// 땅 물체 배치
	CGround* pGround = new CGround;
	pGround->SetPos(Vec2(640.f, 400.f));
	pGround->SetScale(Vec2(500.f, 100.f));
	pGround->SetName(L"Ground");
	AddObject(pGround, GROUP_TYPE::GROUND);



	//타일 로딩
	//LoadTile(L"Tile\\start.tile");

	// 충돌 지정
	// Player 그룹과 Monster 그룹 간의 충돌체크
	// scene은 마지막에 두 그룹 간의 충돌 여부를 검사
	// 충돌 가능한 조합...을 만들어주는 개념 (마치 체크박스처럼)
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER); // 플레이어-몬스터
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::PROJ_PLAYER); // 몬스터-총알
	
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);

	// Camera Start 지정
	// 	   위에 해상도 받아다 몬스터 위치지정 하는 코드 있어서 일단 주석처리
	//Vec2 vResolution = CCore::GetInst()->GetResolution();
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);
	
	// Camera 효과 지정
	CCamera::GetInst()->FadeOut(1.f);
	CCamera::GetInst()->FadeIn(1.f);

	// Scene Enter 말미에 꼭 Start를 넣어주자.
	start();
}

// 충돌조합 체크박스를 해제하듯이 충돌 그룹 해제해주어야 함
// 다음 씬에서는 다른 물체끼리의 충돌 검사할 수 있으니까
void CScene_Start::Exit()
{
	// 씬의 객체 삭제
	DeleteAll();

	// 그룹 충돌 지정 해제
	CCollisionMgr::GetInst()->Reset();
}

void CScene_Start::CreateForce()
{
	m_vForcePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);

	// 강체를 보유한 모든 물체에게 힘을 전달
}


