#include "CWall.h"

#include "CCollider.h"
//#include "CGravity.h"

CWall::CWall()
{
	CreateCollider();
}

CWall::~CWall()
{
}


void CWall::start()
{
	GetCollider()->SetScale(Vec2(GetScale()));
}

void CWall::update()
{
}

void CWall::render(HDC _dc)
{
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();

	Rectangle(_dc
		, (int)(vRenderPos.x)
		, (int)(vRenderPos.y)
		, (int)(vRenderPos.x + vScale.x)
		, (int)(vRenderPos.y + vScale.y));
}

// 충돌체에서 충돌 이벤트 발생 시 실행될 함수
void CWall::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		// Ground 상태임을 알려준다
		//pOtherObj->GetGravity()->SetGround(true);

		// DT로 인해 생기는 오차 보정 (깔끔하게 땅 위로 올라서도록)
		// 오브젝트가 아닌 충돌체 간 비교로 계산해야 한다.
		// Enter뿐만 아니라 OnCollision에서도 동일한 처리 필요함 (발을 땅바닥에 딱 붙이고 있을 때는 enter가 아닌 OnCollision이기 때문에, 파고든 값을 보정해주면 enter에 걸리지 않아서
		// 남은 속도 작용 시 계속 파고들게 됨)

		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();



		// 이전 프레임 플레이어 좌표 가져와서 벽의 어느 면에 닿은건지 비교


		// 파고든 길이
		float fLen = abs(vObjPos.y - vPos.y);
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

		vObjPos = pOtherObj->GetPos();

		vObjPos.y -= fValue;

		pOtherObj->SetPos(vObjPos);
	}
}
	
void CWall::OnCollision(CCollider* _pOther)
{
	// Enter뿐만 아니라 OnCollision에서도 동일한 처리 필요함 (발을 땅바닥에 딱 붙이고 있을 때는 enter가 아닌 OnCollision이기 때문에, 파고든 값을 보정해주면 enter에 걸리지 않아서
	// 남은 속도 작용 시 계속 파고들게 됨)

	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		// Ground 상태임을 알려준다
		//pOtherObj->GetGravity()->SetGround(true);

		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();

		// 파고든 길이
		float fLen = abs(vObjPos.y - vPos.y);
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

		vObjPos = pOtherObj->GetPos();

		vObjPos.y -= fValue;

		// 플레이어 좌표를 세팅
		pOtherObj->SetPos(vObjPos);
	}

	// 미사일과 벽 충돌 시
	if (pOtherObj->GetName() == L"Missile")
	{
		// 미사일 객체 삭제
	}
}

void CWall::OnCollisionExit(CCollider* _pOther)
{
	// 바닥과 충돌 상태가 아닐 때 그라운드 해제

	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		//pOtherObj->GetGravity()->SetGround(false);
	}
}