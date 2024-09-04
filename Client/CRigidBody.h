#pragma once

class CObject;

class CRigidBody
{
private:
	CObject* m_pOwner;
	Vec2 m_vForce;			// ũ��, ����
	Vec2 m_vAccel;			// ���ӵ�
	Vec2 m_vAccelA;			// �߰� ���ӵ� (ex: �߷°��ӵ�)
	Vec2 m_vVelocity;		// �ӵ� (�ӷ°� ����)
	
	Vec2 m_vMaxVelocity;		// �ִ� �ӵ� (2����), Ⱦ��ũ�ѿ� �ش�
	//float m_fMaxVelocity;	// �ִ� �ӵ� (��� �࿡ �ش�), ž��� �ش�
	
	float m_fMass;			// ����
	float m_fFricCoeff;		// ���� ��� (Friction Coefficient)

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
	float GetSpeed() { return m_vVelocity.Length(); }
	Vec2 GetVelocity() { return m_vVelocity; }

	// �ӵ��� ���� ���ӵ��� ���� ��������, �� ������ �ӵ��� ���� ������ �����ؾ� �ϴ� ��찡 �������� ��
	void SetVelocity(Vec2 _vVel) { m_vVelocity = _vVel; }
	void AddVelocity(Vec2 _vVel) { m_vVelocity += _vVel; }

	// �ִ�ӵ� ����
	//void SetMaxVelocity(float _fMaxVel) { m_fMaxVelocity = _fMaxVel; } // ž��
	void SetMaxVelocity(Vec2 _vMaxVel) { m_vMaxVelocity = _vMaxVel; } // Ⱦ��ũ��
	
	// ���� ���ӵ���, ������ �� ��ŭ�� '��'�� �־�������, ������ ����Ͽ�
	// �� ������ ���ӵ��� ����ڴٰ� ����ϴ� �����ε�
	// �߷� ���ӵ��� '���� ���ӵ�' �̹Ƿ�!! ���� �޾ƿ´�.
	void SetAccelAlpha(Vec2 _vAccel) { m_vAccelA = _vAccel; }

private:
	void Move();


public:
	CRigidBody();
	~CRigidBody();

	// ������ ���� Ŭ������ Object�� friend
	friend class CObject;
};

