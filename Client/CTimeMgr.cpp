#include "pch.h"
#include "CTimeMgr.h"

#include "CCore.h"


CTimeMgr::CTimeMgr()
	: m_llCurCount{}
	, m_llPrevCount{} //유니온...?
	, m_llFrequency{}
	, m_dDT(0.)
	, m_iCallCount(0)
	, m_dAcc(0.)
{
}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::init()
{
	// GetTickCount랑 비슷한데 밀리세컨드 단위(1초에 천만)
	// 프레임이랑 관계 없이 딱 시간만 재는 거임!!
	// 현재 카운트 (프로그램 실행~현재까지)
	QueryPerformanceCounter(&m_llPrevCount);

	// 초 당 카운트가 얼마나 발생했는가
	QueryPerformanceFrequency(&m_llFrequency);
}


void CTimeMgr::update()
{
	// update는 매 프레임 마다 호출됨.
	// 따라서, update 호출 전에 잰 시간과 지금 잰 시간의 차이가
	// 1프레임 당 시간이 된다

	QueryPerformanceCounter(&m_llCurCount);

	//QuadPart 부분이 LARGE_INTEGER 자료형의 실제 long long 부분 데이터
	// 정수끼리 나눠봤자 0 나올테니까, float 변환
	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)(m_llFrequency.QuadPart);

	

	// fps = 1. / m_dDT;
	// 이렇게 매번 dt에 따른 fps를 계산하면 dt에 따라 fps가 너무 들쭉날쭉해지는 문제
	// 타임 매니저의 호출 횟수를 세서
	// 정말 1초에 걸린 프레임 수를 세자

	// 다음 프레임 시간 계산 위해 이전 카운트 값을 현재 카운트 값으로 갱신
	m_llPrevCount = m_llCurCount;

	// 디버깅 시 DT가 개커지는 문제
	// DT를 강제로 1/60초로 고정한다
	// 디버그 코드에서만 작동하도록 한다
#ifdef _DEBUG
	if (m_dDT > (1. / 60.))
		m_dDT = (1. / 60.);
#endif
}


void CTimeMgr::render()
{
	++m_iCallCount;
	m_dAcc += m_dDT; // DT 누적 > 프로그램 실행 시간(초 단위)

	if (m_dAcc >= 1.)
	{
		m_iFPS = m_iCallCount;
		m_dAcc = 0.;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS: %d, DT: %f", m_iFPS, m_dDT);
		SetWindowText(CCore::GetInst()->GetMainHwnd(), szBuffer);
	}
}