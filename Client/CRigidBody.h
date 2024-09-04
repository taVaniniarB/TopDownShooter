#pragma once

class CObject;

class CRigidBody
{
private:
	CObject* m_pOwner;
	Vec2 m_vForce;			// 크기, 방향
	Vec2 m_vAccel;			// 가속도
	Vec2 m_vAccelA;			// 추가 가속도 (ex: 중력가속도)
	Vec2 m_vVelocity;		// 속도 (속력과 방향)
	
	Vec2 m_vMaxVelocity;		// 최대 속도 (2차원), 횡스크롤에 해당
	//float m_fMaxVelocity;	// 최대 속도 (모든 축에 해당), 탑뷰애 해당
	
	float m_fMass;			// 질량
	float m_fFricCoeff;		// 마찰 계수 (Friction Coefficient)

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
	float GetSpeed() { return m_vVelocity.Length(); }
	Vec2 GetVelocity() { return m_vVelocity; }

	// 속도란 원래 가속도를 붙인 값이지만, 한 순간에 속도를 일정 값으로 세팅해야 하는 경우가 있을지도 모름
	void SetVelocity(Vec2 _vVel) { m_vVelocity = _vVel; }
	void AddVelocity(Vec2 _vVel) { m_vVelocity += _vVel; }

	// 최대속도 세팅
	//void SetMaxVelocity(float _fMaxVel) { m_fMaxVelocity = _fMaxVel; } // 탑뷰
	void SetMaxVelocity(Vec2 _vMaxVel) { m_vMaxVelocity = _vMaxVel; } // 횡스크롤
	
	// 본래 가속도란, 나에게 이 만큼의 '힘'이 주어졌으니, 질량을 고려하여
	// 이 정도의 가속도가 생기겠다고 계산하는 개념인데
	// 중력 가속도는 '고정 가속도' 이므로!! 따로 받아온다.
	void SetAccelAlpha(Vec2 _vAccel) { m_vAccelA = _vAccel; }

private:
	void Move();


public:
	CRigidBody();
	~CRigidBody();

	// 관련이 깊은 클래스인 Object와 friend
	friend class CObject;
};

