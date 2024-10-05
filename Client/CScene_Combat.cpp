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
	CScene::update(); //부모 코드 재활용

	/*for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
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
	*/

	//if (KEY_TAP(KEY::LBTN))
	//{
	//	// 마우스 좌표를 알아와서 거기를 Lookat으로 잡자
	//	// 마우스 좌표는 렌더링 기준이라, 그걸 진짜좌표로 변환
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

	
	// 충돌 그룹 지정
	// scene은 마지막에 두 그룹 간의 충돌 여부를 검사한다.
	// 충돌 가능한 조합...을 만들어주는 개념 (마치 체크박스처럼)
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::DROPPED_WEAPON); // 떨어진무기-플레이어
	
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_PLAYER, GROUP_TYPE::WALL); // 벽-플레이어 총알
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_PLAYER, GROUP_TYPE::TILE_WALL);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_PLAYER, GROUP_TYPE::CORNER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_MONSTER, GROUP_TYPE::WALL); // 벽-몬스터 총알
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_MONSTER, GROUP_TYPE::TILE_WALL);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PROJ_MONSTER, GROUP_TYPE::CORNER);
	
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::WALL); // 플레이어-벽
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::TILE_WALL);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::CORNER);

	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::WALL); // 몬스터-벽
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::TILE_WALL);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::CORNER);
	
	// 히트박스 - 총알
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::HITBOX_PLAYER, GROUP_TYPE::PROJ_MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::HITBOX_MONSTER, GROUP_TYPE::PROJ_PLAYER);

	// 플레이어 - SC
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::SCENE_CHANGER);
	
	
	// Camera Start 지정
	//Vec2 vResolution = CCore::GetInst()->GetResolution();
	//CCamera::GetInst()->SetLookAt(vResolution / 2.f);
	
	// Camera 효과 지정
	CCamera::GetInst()->FadeIn(FADEIN_TIME);

	// Scene Enter 말미에 꼭 Start를 넣어주자.
	start();
}

// 충돌조합 체크박스를 해제하듯이 충돌 그룹 해제해주어야 함
// 다음 씬에서는 다른 물체끼리의 충돌 검사할 수 있으니까
void CScene_Combat::Exit()
{
	// 씬의 객체 삭제
	DeleteAll();

	// 콤보 초기화
	//CScoreMgr::GetInst()->ResetCombo();

	// 그룹 충돌 지정 해제
	CCollisionMgr::GetInst()->Reset();
}
//
//void CScene_Start::CreateForce()
//{
//	m_vForcePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
//
//	// 강체를 보유한 모든 물체에게 힘을 전달
//}


