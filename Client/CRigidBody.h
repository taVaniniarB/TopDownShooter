#pragma once

class CObject;

class CRigidBody
{
private:
	CObject* m_pOwner;
	Vec2 m_vForce; // 크기, 방향
	Vec2 m_vAccel; // 가속도
	Vec2 m_vVelocity; // 속도 (속력과 방향)
	float m_fMass; // 질량

	// F = M * A
	// 가속도가 누적돼서 속도가 나온다
	// V += A * DT

public:
	void finalUpdate();

public:
	// 이번 프레임에서 받을 힘들을 누적시킨다
	void AddForce(Vec2 _vF) { m_vForce += _vF; }
	void SetMass(float _fMass) { m_fMass = _fMass; }
	float GetMass(){ return m_fMass; }

private:
	void Move();


public:
	CRigidBody();
	~CRigidBody();

	// 관련이 깊은 클래스인 Object와 friend
	friend class CObject;
};

