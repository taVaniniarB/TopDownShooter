#include "pch.h"
#include "CCamera.h"

#include "CObject.h"
#include "CCore.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CResMgr.h"
#include "CTexture.h"

CCamera::CCamera()
	: m_pTargetObj(nullptr)
	, m_fTime(0.5f)
	, m_fSpeed(0.f)
	, m_fAccTime(0.f)
	, m_pVeilTex(nullptr)
	, m_bIsShaking(false)
{
	
}
CCamera::~CCamera()
{

}



void CCamera::Shake(float _fPower, Vec2 _vDir)
{
	if (m_bIsShaking)
	{
		if (m_pTargetObj)
		{
			if (m_pTargetObj->IsDead())
			{
				m_pTargetObj = nullptr; // 죽은 객체 사라져서 null참조되면 안 되니까 해제
			}
			else
			{
				m_tShakeInfo.vShakeDir = _vDir;
				m_tShakeInfo.iCnt = 0;
				m_tShakeInfo.vAccMove = Vec2(0.f, 0.f);
				return;
			}
		}
	}

	m_bIsShaking = true;

	m_tShakeInfo.fDuration = 0.2f;
	m_tShakeInfo.fCurTime = 0.f;
	m_tShakeInfo.fPower = _fPower;
	m_tShakeInfo.iFrequency = 3;
	m_tShakeInfo.iCnt = 1;
	m_tShakeInfo.fPeriod = m_tShakeInfo.fDuration / m_tShakeInfo.iFrequency;
	m_tShakeInfo.vShakeDir = _vDir;
}

// 카메라 객체 생성 시점 신경 쓸 필요 없이 init을 통해 호출하도록 한다
void CCamera::init()
{
	// 카메라가 초기화될 때 VeilTexture를 가지고 있게 한다
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	m_pVeilTex = CResMgr::GetInst()->CreateTexture(L"CameraVeil", (UINT)vResolution.x, (UINT)vResolution.y);
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
	else
	{
		if (KEY_HOLD(KEY::UP))
			m_vLookAt.y -= 500.f * fDT;
		if (KEY_HOLD(KEY::DOWN))
			m_vLookAt.y += 500.f * fDT;
		if (KEY_HOLD(KEY::RIGHT))
			m_vLookAt.x += 500.f * fDT;
		if (KEY_HOLD(KEY::LEFT))
			m_vLookAt.x -= 500.f * fDT;
	}

	if (m_bIsShaking)
	{
		m_tShakeInfo.fCurTime += fDT;
		
		// 사인의 미분 결과인 코사인은 사인 그래프의 기울기를 의미한다.
		// 코사인 그래프의 1주기가 2파이임을 고려해서, 함수의 인자로 넣을 수식을 작성하였다.
		float x = 2 * PI * m_tShakeInfo.fCurTime / m_tShakeInfo.fPeriod;
		float dirSign = cos(x);
		(dirSign > 0) ? (dirSign = 1) : (dirSign = -1);

		// 주기가 끝나면 현재시간을 초기화하고, 카운트를 1 증가시킨다.
		if (m_tShakeInfo.fCurTime >= m_tShakeInfo.fPeriod)
		{
			m_tShakeInfo.fCurTime = 0;
			m_tShakeInfo.iCnt++;
			if (m_tShakeInfo.iCnt > m_tShakeInfo.iFrequency)
				m_bIsShaking = false;
		}

		// 세팅된 방향으로 움직임
		// '한 프레임에서 증가시킬 거리' 만큼 m_vLookAt에 증가
		Vec2 vMove = m_tShakeInfo.vShakeDir * dirSign * m_tShakeInfo.fPower * fDT;
		m_tShakeInfo.vAccMove += vMove;
		// 고정 > 이동 > 고정 > 이동 > 고정 > 이동 ...
		// 그럼...
		// 그래프의 y값만큼 증가시켜주어야 함
		m_vLookAt += m_tShakeInfo.vAccMove;
	}

	// 화면 중앙좌표와 카메라 LookAt 차이 계산
	CalDiff();
}

void CCamera::render(HDC _dc)
{
	if (m_listCamEffect.empty())
		return;

	// CurTime이 Duration을 뛰어넘는 순간, 효과가 끝나도록 구현한다.
	
	// 시간 누적값을 체크
	tCamEffect& effect = m_listCamEffect.front();
	effect.fCurTime += fDT;


	float fRatio = 0; // 이펙트 진행 시간 비율
	fRatio = effect.fCurTime / effect.fDuration; // 이펙트 진행 시간 비율

	fRatio = std::clamp(fRatio, 0.f, 1.f); // dt가 튀어서 비율이 0 초과하는 것 방지

	int iAlpha = 0;

	if (CAM_EFFECT::FADE_OUT == effect.eEffect)
	{
		iAlpha = (int)(fRatio * 255.f);
	}
	else if (CAM_EFFECT::FADE_IN == effect.eEffect)
	{
		iAlpha = (int)((1 - fRatio) * 255.f);
	}
	
	BLENDFUNCTION bf = {};
	bf.BlendOp = AC_SRC_OVER;
	bf.AlphaFormat = 0; // 불러온 리소스와 달리, 소스가 없는 직접만든 텍스처이므로
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = iAlpha; //전역적으로 적용되는 알파!

	// 목적지 dc, 출력위치(좌상단의 x, y), 가로세로, 원본 dc, 원본 이미지의 좌상단 xy, 너비높이, BLANDFUNCTION 구조체
	AlphaBlend(_dc
		, 0, 0
		, (int)(m_pVeilTex->Width())
		, (int)(m_pVeilTex->Height())
		, m_pVeilTex->GetDC()
		, 0, 0
		, (int)(m_pVeilTex->Width()), (int)(m_pVeilTex->Height()), bf);


	// 진행 시간이 이펙트 최대 지정 시간을 넘어선 경우
	// 뒤로 뺀 이유: DT체크를 미룸으로써 duration이 dt를 살짝 넘어도 끝까지 출력되도록 한다
	// 이번 프레임까지는 현재효과를 적용시키도록...
	// 즉 중간에 끊지 않고 완전히 끝낸 다음 pop으로 방출>그 다음 효과가 실행케 함
	if (effect.fDuration < effect.fCurTime)
	{
		// 효과 종료
		m_listCamEffect.pop_front();
	}
}

void CCamera::CalDiff()
{
	m_fAccTime += fDT;

	if (m_pTargetObj)
	{
		if (m_pTargetObj->IsDead())
		{
			m_pTargetObj = nullptr; // 죽은 객체 사라져서 null참조되면 안 되니까 해제
		}
		else
		{
			m_vCurLookAt = m_vLookAt;
		}
	}
	else if (m_fAccTime >= m_fTime)
	{
		// 시간 초과 시, 도착 지점으로 세팅
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
			m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * m_fSpeed * vel * fDT;
		}
	}
	
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2;

	m_vDiff = m_vCurLookAt - vCenter;
	m_vPrevLookAt = m_vCurLookAt;
}