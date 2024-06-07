#pragma once

class CCollider;
class CAnimator;

// ������Ʈ�� ī�޶� �����ؼ�,
// ������ �� �߽���ǥ-ī�޶���ǥ ���̰� �ݿ�
#include "CCamera.h"
// ������� ��������� �������ؼ� �� �Ϸ� ������
// ī�޶�� ������ ��� ������ �׻� ���Ŷ�,
// �� cpp���� ������ �ʿ� ������ �Ѵ�

class CObject
{
private:
	wstring		m_strName;
	Vec2		m_vPos;
	Vec2		m_vScale;

	// Component
	CCollider*	m_pCollider; // nullptr > �浹ü �ʿ����
	CAnimator*	m_pAnimator;

	bool		m_bAlive; // ������Ʈ Ȱ��ȭ üũ

public:
	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }
	
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	
	void SetName(const wstring& _strName) { m_strName = _strName; };
	const wstring& GetName() { return m_strName; }

	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }

	bool IsDead() { return !m_bAlive; }

	void CreateCollider();
	void CreateAnimator();

	// ���� �����Լ�ȭ
	// ��� Object �Ļ� Ŭ�������� �ڱ⸸�� ������Ʈ�� �����ϵ��� �����Ѵ�
	virtual void update() = 0;
	// �������̵� X (�θ������ͷ� ȣ�� �� ����X)
	// �ٵ� �������̵� ��ü�� ���� �� �ƴϱ⵵ �ϰ�
	// �ڽİ�ü �����ͷ� ����Ű�� �������̵� �Լ� ���� ������������
	// ���ʿ� �ڽĿ��� ���� ��ü�� ���ƹ�����
	// virtual, final�� �ٿ��ش�.
	virtual void finalUpdate() final;
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
