#pragma once

class CObject;

class CCollider
{
private:
	// 정적 멤버 (특: 객체에 포함 안되고 항상 데이터 영역에 살아있음)
	static UINT g_iNextID;

	CObject*	m_pOwner;		// Collider 소유하는 오브젝트
	Vec2		m_vOffsetPos;	// 오브젝트로부터 상대적 위치
	Vec2		m_vFinalPos;	// final update에서 매 프레임마다 계산하는 최종값
	Vec2		m_vScale;		// 충돌체 크기

	UINT		m_iID;			// 충돌체 고유한 ID 값
	int			m_iCol;

	bool		m_bActive;		// 충돌체 활성화 여부

public:
	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetOffsetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }

	Vec2 GetFinalPos() { return m_vFinalPos; }

	CObject* GetObj() { return m_pOwner; }

	UINT GetID() { return m_iID; }

public:
	void finalUpdate();
	void render(HDC dc);

public:
	void OnCollision(CCollider* _pOther); // 충돌 중일 경우 호출될 함수
	void OnCollisionEnter(CCollider* _pOther); // 충돌 진입
	void OnCollisionExit(CCollider* _pOther); // 충돌 해제
	
	// collider끼리의 대입 방어
	CCollider& operator = (CCollider& _origin) = delete;

public:
	CCollider();
	CCollider(const CCollider& _origin);
	~CCollider();

	friend class CObject;
};

