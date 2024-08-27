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
{
	
}
CCamera::~CCamera()
{

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

	if (KEY_HOLD(KEY::UP))
		m_vLookAt.y -= 500.f * fDT;
	if (KEY_HOLD(KEY::DOWN))
		m_vLookAt.y += 500.f * fDT;
	if (KEY_HOLD(KEY::RIGHT))
		m_vLookAt.x += 500.f * fDT;
	if (KEY_HOLD(KEY::LEFT))
		m_vLookAt.x -= 500.f * fDT;


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

	// �ð� �ʰ� ��, ���� �������� ����
	if (m_fAccTime >= m_fTime)
	{
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
			m_vCurLookAt = m_vPrevLookAt + vLookDir.Nomalize() * m_fSpeed * vel * fDT;
		}


		

		// ���� LookAt + (����ȭ�� ���⺤��*�ӵ�)
		//m_vCurLookAt = m_vPrevLookAt + vLookDir.Nomalize() * m_fSpeed * fDT;
	}
	
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2;

	m_vDiff = m_vCurLookAt - vCenter;
	m_vPrevLookAt = m_vCurLookAt;

	
}