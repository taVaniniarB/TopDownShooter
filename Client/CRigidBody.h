#pragma once

class CObject;

class CRigidBody
{
private:
	CObject* m_pOwner;
	Vec2 m_vForce; // ũ��, ����
	Vec2 m_vAccel; // ���ӵ�
	Vec2 m_vVelocity; // �ӵ� (�ӷ°� ����)
	float m_fMass; // ����

	// F = M * A
	// ���ӵ��� �����ż� �ӵ��� ���´�
	// V += A * DT

public:
	void finalUpdate();

public:
	// �̹� �����ӿ��� ���� ������ ������Ų��
	void AddForce(Vec2 _vF) { m_vForce += _vF; }
	void SetMass(float _fMass) { m_fMass = _fMass; }
	float GetMass(){ return m_fMass; }

private:
	void Move();


public:
	CRigidBody();
	~CRigidBody();

	// ������ ���� Ŭ������ Object�� friend
	friend class CObject;
};

