#pragma once
#include "global.h"
// 오브젝트는 카메라를 참조해서,
// 랜더링 시 중심좌표-카메라좌표 차이값 반영
#include "CCamera.h"


class CCollider;
class CAnimator;
class CRigidBody;
class CGravity;
class CTexture;


// 헤더에서 헤더참조는 어지간해선 안 하려 하지만
// 카메라는 어차피 모든 옵젝이 항상 쓸거라,
// 매 cpp마다 참조할 필요 없도록 한다

class CObject
{
private:
	wstring		m_strName;
	Vec2		m_vPos;
	Vec2		m_vScale;
	CTexture*	m_pTex;
	UINT		m_iImgIdx;

	// Component
	CCollider*	m_pCollider; // nullptr > 충돌체 필요 없음
	CAnimator*	m_pAnimator; // nullptr > 애니메이션 필요 없음
	CRigidBody* m_pRigidBody; // nullptr > 물리 영향 안 받음
	CGravity*	m_pGravity;

	bool		m_bAlive; // 오브젝트 활성화 체크
	bool		m_bEnabled; // 오브젝트 업데이트 동작 여부

public:
	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }
	CTexture* GetTexture() { return m_pTex; }
	
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	void SetTexture(CTexture* _pTex) { m_pTex = _pTex; }
	void SetImgIdx(UINT _i) { m_iImgIdx = _i; }
	
	void SetName(const wstring& _strName) { m_strName = _strName; };
	const wstring& GetName() { return m_strName; }

	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }
	CRigidBody* GetRigidBody() { return m_pRigidBody; }
	CGravity* GetGravity() { return m_pGravity; }

	bool IsDead() { return !m_bAlive; }

	void SetEnabled(bool _b) { m_bEnabled = _b; }
	bool GetEnabled() { return m_bEnabled; }

	void CreateCollider();
	void CreateAnimator();
	void CreateRigidBody();
	void CreateGravity();

	virtual void start() {} // Scene이 시작되기 전에 호출되는 함수
	// Scene이 시작되는 순간 할 일이 있는 obj들은 이 함수를 오버라이딩 하도록 한다.
	
	// 순수 가상함수화
	// 모든 Object 파생 클래스들은 자기만의 업데이트를 구현하도록 강제한다
	virtual void update() = 0;
	// 오버라이딩 X (부모포인터로 호출 시 실행X)
	// 근데 오버라이딩 자체가 막힌 건 아니기도 하고
	// 자식객체 포인터로 가리키면 오버라이딩 함수 실행 가능해져버림
	// 애초에 자식에서 구현 자체를 막아버리자
	// virtual, final을 붙여준다.
	virtual void finalUpdate();
	virtual void render(HDC _dc);

private: // 오로지 이벤트 처리를 통해서만 오브젝트를 죽일 수 있도록 함
	void SetDead() { m_bAlive = false; }


public:
	virtual void OnCollision(CCollider* _pOther) {}
	virtual void OnCollisionEnter(CCollider* _pOther) {}
	virtual void OnCollisionExit(CCollider* _pOther) {}

	void component_render(HDC _dc);

	// 인터페이스화
	// 자기 자신의 복제 버전을 되돌려주는 함수
	virtual CObject* Clone() = 0;

public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();

	friend class CEventMgr;
};

