#pragma once
class CTimeMgr
{
	SINGLE(CTimeMgr);
private:
	//프레임 당 시간(델타 타임)을 구해보자
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llFrequency;

	double			m_dDT; // 델타 타임. 프레임 사이의 시간
	double			m_dAcc; // 1초 체크를 위한 DT 누적(초 단위의 실행시간)
	UINT			m_iCallCount; // 초 당 타임매니저 함수 호출 횟수(프레임 수) 체크
	UINT			m_iFPS; //초 당 호출 횟수

public:
	void init();
	void update();
	void render();

public:
	double GetDT() { return m_dDT; }
	float GetfDT() { return (float)m_dDT; }
};

