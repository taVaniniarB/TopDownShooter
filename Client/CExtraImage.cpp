#include "pch.h"
#include "CExtraImage.h"
#include "CResMgr.h"

CExtraImage::CExtraImage(Vec2 vPos, Vec2 vScale)
	: m_pImage(nullptr)
{
	SetPos(vPos);
	SetScale(vScale);
}

CExtraImage::~CExtraImage()
{
}

void CExtraImage::update()
{
}

void CExtraImage::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();

	// GDI+ÀÇ Graphics °´Ã¼ »ý¼º
	Gdiplus::Graphics graphics(_dc);

	graphics.DrawImage(m_pImage,
		(int)(vPos.x - vScale.x / 2.f),
		(int)(vPos.y - vScale.y / 2.f),
		(int)(vScale.x),
		(int)(vScale.y));
}

void CExtraImage::SetImage(wstring _imageName, wstring _imagePath)
{
	Image* pImage = CResMgr::GetInst()->LoadGdiImage(_imageName, _imagePath);
	m_pImage = pImage;
}