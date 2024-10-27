#pragma once
#include "CObject.h"

class CExtraImage:
	public CObject
{
public:
	CExtraImage(Vec2 vPos, Vec2 vSacle);
	~CExtraImage();

	CLONE(CExtraImage);

private:
	Image* m_pImage;

public:
	void update();
	void render(HDC _dc);

public:
	void SetImage(wstring _imageName, wstring _imagePath);
};

