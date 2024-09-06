#pragma once
#include "global.h"
// ������Ʈ�� ī�޶� �����ؼ�,
// ������ �� �߽���ǥ-ī�޶���ǥ ���̰� �ݿ�
#include "CCamera.h"


class CCollider;
class CAnimator;
class CRigidBody;
class CGravity;
class CTexture;


// ������� ��������� �������ؼ� �� �Ϸ� ������
// ī�޶�� ������ ��� ������ �׻� ���Ŷ�,
// �� cpp���� ������ �ʿ� ������ �Ѵ�

class CObject
{
private:
	wstring		m_strName;
	Vec2		m_vPos;
	Vec2		m_vScale;
	CTexture* m_pTex;

	// Component
	CCollider*	m_pCollider; // nullptr > �浹ü �ʿ� ����
	CAnimator*	m_pAnimator; // nullptr > �ִϸ��̼� �ʿ� ����
	CRigidBody* m_pRigidBody; // nullptr > ���� ���� �� ����
	CGravity*	m_pGravity;

	bool		m_bAlive; // ������Ʈ Ȱ��ȭ üũ

public:
	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }
	CTexture* GetTexture() { return m_pTex; }
	
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	void SetTexture(CTexture* _pTex) { m_pTex = _pTex; }
	
	void SetName(const wstring& _strName) { m_strName = _strName; };
	const wstring& GetName() { return m_strName; }

	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }
	CRigidBody* GetRigidBody() { return m_pRigidBody; }
	CGravity* GetGravity() { return m_pGravity; }

	bool IsDead() { return !m_bAlive; }

	void CreateCollider();
	void CreateAnimator();
	void CreateRigidBody();
	void CreateGravity();

	virtual void start() {} // Scene�� ���۵Ǳ� ���� ȣ��Ǵ� �Լ�
	// Scene�� ���۵Ǵ� ���� �� ���� �ִ� obj���� �� �Լ��� �������̵� �ϵ��� �Ѵ�.
	
	// ���� �����Լ�ȭ
	// ��� Object �Ļ� Ŭ�������� �ڱ⸸�� ������Ʈ�� �����ϵ��� �����Ѵ�
	virtual void update() = 0;
	// �������̵� X (�θ������ͷ� ȣ�� �� ����X)
	// �ٵ� �������̵� ��ü�� ���� �� �ƴϱ⵵ �ϰ�
	// �ڽİ�ü �����ͷ� ����Ű�� �������̵� �Լ� ���� ������������
	// ���ʿ� �ڽĿ��� ���� ��ü�� ���ƹ�����
	// virtual, final�� �ٿ��ش�.
	virtual void finalUpdate();
	virtual void render(HDC _dc);

private: // ������ �̺�Ʈ ó���� ���ؼ��� ������Ʈ�� ���� �� �ֵ��� ��
	void SetDead() { m_bAlive = false; }


public:
	virtual void OnCollision(CCollider* _pOther) {}
	virtual void OnCollisionEnter(CCollider* _pOther) {}
	virtual void OnCollisionExit(CCollider* _pOther) {}

	void component_render(HDC _dc);

	// �������̽�ȭ
	// �ڱ� �ڽ��� ���� ������ �ǵ����ִ� �Լ�
	virtual CObject* Clone() = 0;

public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();

	friend class CEventMgr;
};

