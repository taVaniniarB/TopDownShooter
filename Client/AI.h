#pragma once

class CMonster;
class CState;
// 기능적이지 않음
// 다양한 state들을 보유하고 변경하는 역할

class AI
{
private:
	map<MON_STATE, CState*> m_mapState; // enum을 키값으로 삼는다
	CState* m_pCurState; // 현재상태
	CMonster*	m_pOwner;


public:
	void update();
	
public:
	void AddState(CState* _pState);
	CState* GetState(MON_STATE _eSTate);
	void SetCurState(MON_STATE _eState);
	void ChangeState(MON_STATE _eNextState);

	CMonster* GetOwner() { return m_pOwner; }
	MON_STATE GetCurState();

public:
	AI();
	~AI();

	friend class CMonster;
};

