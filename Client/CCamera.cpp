#include "pch.h"
#include "CCamera.h"

#include "CObject.h"
#include "CCore.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

CCamera::CCamera()
	: m_pTargetObj(nullptr)
	, m_fTime(0.5f)
	, m_fSpeed(0.f)
	, m_fAccTime(0.f)
{

}
CCamera::~CCamera()
{

}



void CCamera::update()
{
	if (m_pTargetObj)
	{
		if (m_pTargetObj->IsDead())
		{
			m_pTargetObj = nullptr; // 죽은 객체 사라져서 null참조되면 안 되니까 해제
		}
		else
		{
			m_vLookAt = m_pTargetObj->GetPos();
		}
	}

	if (KEY_HOLD(KEY::UP))
		m_vLookAt.y -= 500.f * fDT;
	if (KEY_HOLD(KEY::DOWN))
		m_vLookAt.y += 500.f * fDT;
	if (KEY_HOLD(KEY::RIGHT))
		m_vLookAt.x += 500.f * fDT;
	if (KEY_HOLD(KEY::LEFT))
		m_vLookAt.x -= 500.f * fDT;


	// 화면 중앙좌표와 카메라 LookAt 차이 계산
	CalDiff();
}

void CCamera::CalDiff()
{
	m_fAccTime += fDT;

	// 시간 초과 시, 도착 지점으로 세팅
	if (m_fAccTime >= m_fTime)
	{
		m_vCurLookAt = m_vLookAt;
	}
	else
	{
		// 쫓아갈 방향 벡터 계산
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;

		// 영벡터 발생 방어
		if (!vLookDir.isZero())
		{
		// 등가속도 운동
		// 속도-시간 그래프의 기울기(가속도의 크기)를 4로 고정해서
		// 시간은 AccTime으로 하여, 속도 v를 구한다.
		// 또한, 거속시 공식에 따라 시간 값이 커야 속도가 느려지므로, 빠름>느림 으로 하기 위해 시간값을 m_fTime - m_fAccTime 이렇게 계산해줌
			float vel = 4.f * (m_fTime - m_fAccTime);

			// 이전 위치 + ( 방향 * (속력 * 가속도) * DT )
			m_vCurLookAt = m_vPrevLookAt + vLookDir.Nomalize() * m_fSpeed * vel * fDT;
		}


		

		// 이전 LookAt + (정규화한 방향벡터*속도)
		//m_vCurLookAt = m_vPrevLookAt + vLookDir.Nomalize() * m_fSpeed * fDT;
	}
	
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2;

	m_vDiff = m_vCurLookAt - vCenter;
	m_vPrevLookAt = m_vCurLookAt;

	
}