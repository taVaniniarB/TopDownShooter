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
				m_pTargetObj = nullptr; // ���� ��ü ������� null�����Ǹ� �� �Ǵϱ� ����
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

// ī�޶� ��ü ���� ���� �Ű� �� �ʿ� ���� init�� ���� ȣ���ϵ��� �Ѵ�
void CCamera::init()
{
	// ī�޶� �ʱ�ȭ�� �� VeilTexture�� ������ �ְ� �Ѵ�
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	m_pVeilTex = CResMgr::GetInst()->CreateTexture(L"CameraVeil", (UINT)vResolution.x, (UINT)vResolution.y);
}

void CCamera::update()
{
	if (m_pTargetObj)
	{
		if (m_pTargetObj->IsDead())
		{
			m_pTargetObj = nullptr; // ���� ��ü ������� null�����Ǹ� �� �Ǵϱ� ����
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
		
		// ������ �̺� ����� �ڻ����� ���� �׷����� ���⸦ �ǹ��Ѵ�.
		// �ڻ��� �׷����� 1�ֱⰡ 2�������� ����ؼ�, �Լ��� ���ڷ� ���� ������ �ۼ��Ͽ���.
		float x = 2 * PI * m_tShakeInfo.fCurTime / m_tShakeInfo.fPeriod;
		float dirSign = cos(x);
		(dirSign > 0) ? (dirSign = 1) : (dirSign = -1);

		// �ֱⰡ ������ ����ð��� �ʱ�ȭ�ϰ�, ī��Ʈ�� 1 ������Ų��.
		if (m_tShakeInfo.fCurTime >= m_tShakeInfo.fPeriod)
		{
			m_tShakeInfo.fCurTime = 0;
			m_tShakeInfo.iCnt++;
			if (m_tShakeInfo.iCnt > m_tShakeInfo.iFrequency)
				m_bIsShaking = false;
		}

		// ���õ� �������� ������
		// '�� �����ӿ��� ������ų �Ÿ�' ��ŭ m_vLookAt�� ����
		Vec2 vMove = m_tShakeInfo.vShakeDir * dirSign * m_tShakeInfo.fPower * fDT;
		m_tShakeInfo.vAccMove += vMove;
		// ���� > �̵� > ���� > �̵� > ���� > �̵� ...
		// �׷�...
		// �׷����� y����ŭ ���������־�� ��
		m_vLookAt += m_tShakeInfo.vAccMove;
	}

	// ȭ�� �߾���ǥ�� ī�޶� LookAt ���� ���
	CalDiff();
}

void CCamera::render(HDC _dc)
{
	if (m_listCamEffect.empty())
		return;

	// CurTime�� Duration�� �پ�Ѵ� ����, ȿ���� �������� �����Ѵ�.
	
	// �ð� �������� üũ
	tCamEffect& effect = m_listCamEffect.front();
	effect.fCurTime += fDT;


	float fRatio = 0; // ����Ʈ ���� �ð� ����
	fRatio = effect.fCurTime / effect.fDuration; // ����Ʈ ���� �ð� ����

	fRatio = std::clamp(fRatio, 0.f, 1.f); // dt�� Ƣ� ������ 0 �ʰ��ϴ� �� ����

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
	bf.AlphaFormat = 0; // �ҷ��� ���ҽ��� �޸�, �ҽ��� ���� �������� �ؽ�ó�̹Ƿ�
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = iAlpha; //���������� ����Ǵ� ����!

	// ������ dc, �����ġ(�»���� x, y), ���μ���, ���� dc, ���� �̹����� �»�� xy, �ʺ����, BLANDFUNCTION ����ü
	AlphaBlend(_dc
		, 0, 0
		, (int)(m_pVeilTex->Width())
		, (int)(m_pVeilTex->Height())
		, m_pVeilTex->GetDC()
		, 0, 0
		, (int)(m_pVeilTex->Width()), (int)(m_pVeilTex->Height()), bf);


	// ���� �ð��� ����Ʈ �ִ� ���� �ð��� �Ѿ ���
	// �ڷ� �� ����: DTüũ�� �̷����ν� duration�� dt�� ��¦ �Ѿ ������ ��µǵ��� �Ѵ�
	// �̹� �����ӱ����� ����ȿ���� �����Ű����...
	// �� �߰��� ���� �ʰ� ������ ���� ���� pop���� ����>�� ���� ȿ���� ������ ��
	if (effect.fDuration < effect.fCurTime)
	{
		// ȿ�� ����
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
			m_pTargetObj = nullptr; // ���� ��ü ������� null�����Ǹ� �� �Ǵϱ� ����
		}
		else
		{
			m_vCurLookAt = m_vLookAt;
		}
	}
	else if (m_fAccTime >= m_fTime)
	{
		// �ð� �ʰ� ��, ���� �������� ����
		m_vCurLookAt = m_vLookAt;
	}
	else
	{
		// �Ѿư� ���� ���� ���
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;

		// ������ �߻� ���
		if (!vLookDir.isZero())
		{
		// ��ӵ� �
		// �ӵ�-�ð� �׷����� ����(���ӵ��� ũ��)�� 4�� �����ؼ�
		// �ð��� AccTime���� �Ͽ�, �ӵ� v�� ���Ѵ�.
		// ����, �żӽ� ���Ŀ� ���� �ð� ���� Ŀ�� �ӵ��� �������Ƿ�, ����>���� ���� �ϱ� ���� �ð����� m_fTime - m_fAccTime �̷��� �������
			float vel = 4.f * (m_fTime - m_fAccTime);

			// ���� ��ġ + ( ���� * (�ӷ� * ���ӵ�) * DT )
			m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * m_fSpeed * vel * fDT;
		}
	}
	
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2;

	m_vDiff = m_vCurLookAt - vCenter;
	m_vPrevLookAt = m_vCurLookAt;
}