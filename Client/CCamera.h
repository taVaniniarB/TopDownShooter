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
	CAM_EFFECT	eEffect;	//ī�޶� ȿ��
	float		fDuration;	// �ִ� ���ӽð�
	float		fCurTime;	// ���� ����ð�
};

struct tCamShakeInfo
{
	float	fDuration;  // ���ӽð�
	float	fCurTime;	// ���� ����ð�
	float	fPower;		// ����
	int		iFrequency;	// ���� ��
	int		iCnt;		// ���� ���� Ƚ��
	float	fPeriod;	// �ֱ�
	Vec2	vShakeDir;	// ����
	Vec2	vAccMove; // ���� �̵���
};



class CCamera
{
	SINGLE(CCamera);
private:
	Vec2		m_vLookAt;		// ī�޶� ��ġ ��ǥ (��ǥ ��ǥ)
	Vec2		m_vCurLookAt;	// ������ġ�� ������ġ ���� ��ġ (���� ī�޶� ��ġ)
	Vec2		m_vPrevLookAt;	// ī�޶��� ���� ������ ��ġ
	// �ް��� ī�޶� �̵� �߻� ��, ƽ �ϰ� �̵����� �ʰ� õõ�� �̵��ϴ� ȿ��
	
	CObject*	m_pTargetObj;	// ī�޶� Ÿ�� ������Ʈ (�Ѿư� ������Ʈ)
	Vec2		m_vDiff;		// �ػ� �߽� ��ġ�� ī�޶� LookAt �� ���̰�

	float		m_fTime;
	float		m_fSpeed;		// Ÿ�� ���󰡴� �ӵ�
	float		m_fAccTime;

	list<tCamEffect> m_listCamEffect;
	CTexture* m_pVeilTex;		// ī�޶� ������ �ؽ�ó (������)

	// shaking�� ���� ���
	bool m_bIsShaking;
	tCamShakeInfo m_tShakeInfo;
	

public:
	// �̵� ��ǥ ��ǥ, (�� ��) �̵��Ÿ�(speed), �����ð��� �����Ѵ�
	void SetLookAt(Vec2 _vLook)
	{
		m_vLookAt = _vLook;

		// ���-���� ��ǥ '����'�� ���ϱ�
		float fMoveDist = (m_vLookAt - m_vPrevLookAt).Length();
		
		
		m_fSpeed = fMoveDist / m_fTime;
		m_fAccTime = 0.f;
	}
	void SetTarget(CObject* _pTarget) { m_pTargetObj = _pTarget; }
	Vec2 GetLookAt() { return m_vCurLookAt; }
	Vec2 GetRenderPos(Vec2 _vObjPos) { return _vObjPos - m_vDiff; }
	// ��������ǥ > ��¥��ǥ ��ȯ
	Vec2 GetRealPos(Vec2 _vRenderPos) { return _vRenderPos + m_vDiff; }

	// Dir �������� f��ŭ ��鸲
	void Shake(float _fPower, Vec2 _vDir);

	void FadeIn(float _fDuration) // �����
	{
		tCamEffect ef = {};
		ef.eEffect = CAM_EFFECT::FADE_IN;
		ef.fDuration = _fDuration;
		ef.fCurTime = 0.f;

		m_listCamEffect.push_back(ef);

		if (0.f == _fDuration)
			assert(nullptr);
	}
	void FadeOut(float _fDuration) // ��ο���
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

