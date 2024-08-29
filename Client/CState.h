#pragma once
class AI;
class CMonster;

class CState
{
private:
	AI*			m_pAI;
	MON_STATE	m_eState;

public:
	AI* GetAI() { return m_pAI; }
	MON_STATE GetType() { return m_eState; }
	CMonster* GetMonster();

public:
	virtual void update() = 0;
	virtual void Enter() = 0;
	virtual void Exit() = 0;

public:
	// CState는 기본 생성자를 없애고, MON_STATE를 입력받는 버전만 있도록 한다.
	// 생성자 있으면 기본 생성자 자동으로 안 만들어줌
	CState(MON_STATE _eState);
	virtual ~CState(); // '상태'는 상속 구조로 만들 것

	friend class AI;
};

