#include "pch.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"

CCollisionMgr::CCollisionMgr()
	: m_arrCheck{}
{}

CCollisionMgr::~CCollisionMgr()
{}

void CCollisionMgr::update()
{
	// 반복문으로 배열 확인하며, 1 체크 돼 있다면 이 그룹과 이 그룹이 충돌이구나
	// 하고 거를 수 있어야 함
	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; ++iRow)
	{
		// 절반만 검사할 것이기 때문에, 별찍기하듯이 생각해서
		// for문이 0부터 시작이 아니라 iRow부터 시작임!!
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				// 현제 씬의 멤버로 오브젝트목록들이 있지
				// 그룹 단위로 요청
				// 작은 값인 row를 앞에, 큰 값인 col을 뒤애 두어 순서 보장
				CollisionUpdateGroup((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);
			}
		}
	}
}

void CCollisionMgr::CollisionUpdateGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	
	// 레퍼런스란 별칭 / 동일시의 개념
	// vecLeft를 GetGroupObject 반환결과 벡터와 동일시 하겠다
	// 반환된 애가 const니까 const 붙여줌
	// 레퍼런스를 지역변수에 저장할 때는 항상 주의하도록 하자
	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRight);

	map<ULONGLONG, bool>::iterator iter;

	// 기억할 점
	// 같은 그룹이 들어왔을 경우, 나 자신과의 충돌 예외설정
	// 해당 오브젝트에 충돌체가 없을 경우

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// 해당 오브젝트가 충돌체 미보유 시 패스
		if (nullptr == vecLeft[i]->GetCollider())
		{
			continue;
		}

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// 충돌체가 없거나, 자기 자신과의 충돌 경우 패스
			if (nullptr == vecRight[j]->GetCollider()
				|| vecLeft[i] == vecRight[j])
			{
				continue;
			}

			CCollider* pLeftCol = vecLeft[i]->GetCollider();
			CCollider* pRightCol = vecRight[j]->GetCollider();

			// 각 충돌체의 ID를 가져와서, 두 충돌체 조합 아이디 생성
			COLLIDER_ID ID;
			ID.iLeft_id = pLeftCol->GetID();
			ID.iRight_id = pRightCol->GetID();
			ID.ID; // 이것을 맵의 key로 쓰겠다

			// 두 충돌체 간의 충돌 여부를 저장하는 노드 가리키는 이터레이터
			iter = m_mapColInfo.find(ID.ID);

			// 가 end iterator라는 것은, 최초 조회라 데이터가 없다는 뜻이므로 데이터 추가
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}
			// iter는 두 충돌체의 충돌 여부를 저장하는 노드를 가리킨다

			// 두 오브젝트의 충돌체를 충돌체크 함수에 던져주자
			if (IsCollision(pLeftCol, pRightCol))
			{
				// 현재 충돌 중이다

				if (iter->second)
				{// 이전에도 충돌 중이었다

					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						// 근데 둘 중 하나가 삭제 예정일 시, 충돌 해제시켜줌
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
					
				}
				else
				{// 이전에는 충돌하지 않는데 이번에 충돌함 (충돌진입시작)

					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead())
					{ // 둘 다 dead가 아닐 때, 충돌 Enter 실행
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
				}
			}
			else // 현재 충돌하지 않음
			{
				if (iter->second)
				{// 이전에는 충돌하고 있었다
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}

// 좌표를 조회하여 진짜 충돌 한다 안 한다를 알려주는 함수
bool CCollisionMgr::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();

	if (abs(vRightPos.x - vLeftPos.x) < (vLeftScale.x + vRightScale.x) / 2.f
		&& abs(vRightPos.y - vLeftPos.y) < (vLeftScale.y + vRightScale.y) / 2.f)
	{
		return true;
	}

	// 두 중심좌표의 x, y 둘 다 scale / 2 미만 만큼 떨어져 있을 때
	// 두 좌표의 차이가 (left.scale / 2 + right.scale / 2) / 2 미만
	return false;
}



// 충돌 대상 테이블 관리
void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	// 더 큰 값의 그룹 타입이 열

	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

	if (iCol < iRow)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	// 그 자리의 비트가 참인가?
	if (m_arrCheck[iRow] & (1 << iCol))
	{
		// 비트빼기 (체크박스 한 번 더 클릭해서 해제되듯)
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		// 비트 넣어주기
		m_arrCheck[iRow] |= (1 << iCol);
	}
}
