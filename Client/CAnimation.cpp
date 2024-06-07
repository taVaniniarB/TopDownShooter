#include "pch.h"
#include "CAnimation.h"

#include "CTimeMgr.h"

#include "CAnimator.h"
#include "CTexture.h"
#include "CObject.h"
#include "CCamera.h"

CAnimation::CAnimation()
	: m_pAnimator(nullptr)
	, m_pTex(nullptr)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::update()
{
	if (m_bFinish)
		return;

	m_fAccTime += fDT;

	if (m_fAccTime > m_vecFrm[m_iCurFrm].fDuration)
	{
		++m_iCurFrm;

		
		if (m_vecFrm.size() <= m_iCurFrm) // 마지막 프레임 도달
		{
			m_iCurFrm = -1; // 끝났다는 뜻으로 -1
			m_bFinish = true;
			m_fAccTime = 0.f;
			return;
		}

		// 누적시간 초기화
		// dt를 누적하다보면 지정 시간을 초과할 때가 있음
		// 그 시간을 보간하기 위해 0으로 초기화하지 않고 누적시간 - 1f당 시간으로 초기화
		// 약간.. 일정 시간 당 애니메이션 실행횟수 일정하게 유지하듯이 한다는 개념
		// 인데, 또 다른 문제점: 한순간 렉이 걸린다면 dt가 개커져서, 렉 풀리는 순간 개빨라짐
		// 걍 냅두기로!
		m_fAccTime = m_fAccTime - m_vecFrm[m_iCurFrm].fDuration;

	}
}

void CAnimation::render(HDC _dc)
{
	if (m_bFinish)
		return;

	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();

	vPos += m_vecFrm[m_iCurFrm].vOffset;

	// 랜더링용 좌표로 변환
	vPos = CCamera::GetInst()->GetRenderPos(vPos);

	// dc, 출력될 위치(오브젝트가 출력될 좌상단 위치), 가로세로길이, 텍스처의 DC, 텍스처 내에서 자를 위치, 가로세로길이
	TransparentBlt(_dc
		, (int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.f)
		, (int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.y / 2.f)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
		, m_pTex->GetDC()					
		, (int)(m_vecFrm[m_iCurFrm].vLT.x)
		, (int)(m_vecFrm[m_iCurFrm].vLT.y)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
		, RGB(0, 255, 0));
}

void CAnimation::Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	m_pTex = _pTex;
	tAnimFrm frm = {};
	for (UINT i = 0; i < _iFrameCount; ++i)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vSliceSize;
		frm.vLT = _vLT + _vStep * (float)i; // 벡터와 int의 덧셈 오버로딩 (벡터-int 순서 맞춰주기)

		m_vecFrm.push_back(frm);
	}
}
