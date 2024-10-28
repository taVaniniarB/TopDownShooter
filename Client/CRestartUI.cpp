#include "pch.h"
#include "CRestartUI.h"
#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CCore.h"

CRestartUI::CRestartUI()
	: m_fCurTime(0.f)
	, m_fAmplitude(10.f)
	, m_fFrequency(2.f)
	, m_fAngle(0.f)
	, m_vBGOffset(Vec2(0.f, 0.f))
{
	Image * pImage = CResMgr::GetInst()->LoadGdiImage(L"RestartUIbg", L"texture\\UI\\blackBG.png");
	m_pBackground = pImage;

	pImage = CResMgr::GetInst()->LoadGdiImage(L"RestartText", L"texture\\UI\\RestartText.png");
	m_pImg = pImage;

	SetPos(Vec2(200.f, CCore::GetInst()->GetHeight() - 50.f));
}

CRestartUI::~CRestartUI()
{
}

void CRestartUI::update()
{
	static float fCurTime = 0.f;
	
	fCurTime += fDT;
	
	m_fAngle = m_fAmplitude * sin(m_fFrequency * fCurTime); // 각 프레임의 각도
}

void CRestartUI::render(HDC _dc)
{
	Vec2 vPos = GetPos() + m_vBGOffset;
	Vec2 vScale = Vec2(m_pBackground->GetWidth() / 4.f, m_pBackground->GetHeight() / 4.f);

	// GDI+의 Graphics 객체 생성
	Gdiplus::Graphics graphics(_dc);

	graphics.DrawImage(m_pBackground,
		(int)(vPos.x - vScale.x / 2.f),
		(int)(vPos.y - vScale.y / 2.f),
		(int)(vScale.x),
		(int)(vScale.y));



	vPos = GetPos();
	vScale = Vec2(m_pImg->GetWidth() / 4.f, m_pImg->GetHeight() / 4.f);

	// 회전의 기준점을 이미지 중심으로 설정
	int centerX = (int)(vPos.x);
	int centerY = (int)(vPos.y);
	// GDI+ 변환 적용: 중심으로 이동 -> 회전 -> 원래 위치로 돌아옴
	graphics.TranslateTransform(centerX, centerY);
	graphics.RotateTransform(m_fAngle);
	graphics.TranslateTransform(-centerX, -centerY);


	graphics.DrawImage(m_pImg,
		(int)(vPos.x - vScale.x / 2.f),
		(int)(vPos.y - vScale.y / 2.f),
		(int)(vScale.x),
		(int)(vScale.y));
}
