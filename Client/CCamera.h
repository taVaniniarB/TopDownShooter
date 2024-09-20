#pragma once

class CObject;

class CTexture;

enum class CAM_EFFECT
{
	FADE_IN,
	FADE_OUT,
	NONE,
};

struct tCamEffect
{
	CAM_EFFECT	eEffect;	//카메라 효과
	float		fDuration;	// 최대 지속시간
	float		fCurTime;	// 현재 진행시간
};

struct tCamShakeInfo
{
	float	fDuration;  // 지속시간
	float	fCurTime;	// 현재 진행시간
	float	fPower;		// 세기
	int		iFrequency;	// 진동 수
	int		iCnt;		// 현재 진동 횟수
	float	fPeriod;	// 주기
	Vec2	vShakeDir;	// 각도
	Vec2	vAccMove; // 누적 이동량
};



class CCamera
{
	SINGLE(CCamera);
private:
	Vec2		m_vLookAt;		// 카메라 위치 좌표 (목표 좌표)
	Vec2		m_vCurLookAt;	// 이전위치와 현재위치 보정 위치 (현재 카메라 위치)
	Vec2		m_vPrevLookAt;	// 카메라가의 이전 프레임 위치
	// 급격한 카메라 이동 발생 시, 틱 하고 이동하지 않고 천천히 이동하는 효과
	
	CObject*	m_pTargetObj;	// 카메라 타겟 오브젝트 (쫓아갈 오브젝트)
	Vec2		m_vDiff;		// 해상도 중심 위치와 카메라 LookAt 간 차이값

	float		m_fTime;
	float		m_fSpeed;		// 타겟 따라가는 속도
	float		m_fAccTime;

	list<tCamEffect> m_listCamEffect;
	CTexture* m_pVeilTex;		// 카메라 가림막 텍스처 (검은색)

	// shaking에 대한 멤버
	bool m_bIsShaking;
	tCamShakeInfo m_tShakeInfo;
	

public:
	// 이동 목표 좌표, (초 당) 이동거리(speed), 누적시간을 세팅한다
	void SetLookAt(Vec2 _vLook)
	{
		m_vLookAt = _vLook;

		// 출발-도착 좌표 '길이'값 구하기
		float fMoveDist = (m_vLookAt - m_vPrevLookAt).Length();
		
		
		m_fSpeed = fMoveDist / m_fTime;
		m_fAccTime = 0.f;
	}
	void SetTarget(CObject* _pTarget) { m_pTargetObj = _pTarget; }
	Vec2 GetLookAt() { return m_vCurLookAt; }
	Vec2 GetRenderPos(Vec2 _vObjPos) { return _vObjPos - m_vDiff; }
	// 렌더링좌표 > 진짜좌표 변환
	Vec2 GetRealPos(Vec2 _vRenderPos) { return _vRenderPos + m_vDiff; }

	// Dir 방향으로 f만큼 흔들림
	void Shake(float _fPower, Vec2 _vDir);

	void FadeIn(float _fDuration) // 밝아짐
	{
		tCamEffect ef = {};
		ef.eEffect = CAM_EFFECT::FADE_IN;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;

		m_listCamEffect.push_back(ef);

		if (0.f == _fDuration)
			assert(nullptr);
	}
	void FadeOut(float _fDuration) // 어두워짐
	{
		tCamEffect ef = {};
		ef.eEffect = CAM_EFFECT::FADE_OUT;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;

		m_listCamEffect.push_back(ef);

		if (0.f == _fDuration)
			assert(nullptr);
	}

public:
	void init();
	void update();
	void render(HDC _dc);

private:
	void CalDiff();

};

