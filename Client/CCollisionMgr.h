#pragma once

class CCollider;

union COLLIDER_ID
{
	struct {
		UINT iLeft_id;
		UINT iRight_id;
	};
	ULONGLONG ID; // id는 음수 불가하므로 unsigned
};

class CCollisionMgr
{
	SINGLE(CCollisionMgr)
private:
	map<ULONGLONG, bool> m_mapColInfo;			// 충돌체 간의 이전 프레임 충돌정보
	UINT	m_arrCheck[(UINT)GROUP_TYPE::END];	// 그룹 간의 충돌 체크 매트릭스
public:
	void update();
	void CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	void Reset()
	{ // 함수 이용해서 배열을 0으로 만들어줌
		memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::END);
	}
private:
	void CollisionUpdateGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight); // 그룹 간의 충돌 갱신
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);
};

