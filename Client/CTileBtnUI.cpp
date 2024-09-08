#include "pch.h"
#include "CTileBtnUI.h"

#include "CTexture.h"

#include "SelectGDI.h"

CTileBtnUI::CTileBtnUI()
{
}

CTileBtnUI::~CTileBtnUI()
{
}

void CTileBtnUI::render(HDC _dc)
{
	CTexture* pTex = GetTexture();

	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	if (pTex)
	{
		UINT iWidth = pTex->Width();
		UINT iHeight = pTex->Height();

		// 타일 파일 높이에 따른 최대 열 개수
		UINT iMaxCol = iWidth / TILE_SIZE;

		//행 개수(쓰는 이유: 인덱스 초과 예외처리)
		UINT iMaxRow = iHeight / TILE_SIZE;

		UINT iCurRow = (UINT)m_iIdx / iMaxCol;
		UINT iCurCol = (UINT)m_iIdx % iMaxCol;

		// 이미지 범위를 넘어선 인덱스
		if (iMaxCol <= iCurCol)
			assert(nullptr);



		TransparentBlt(_dc
			, (int)(vPos.x)
			, (int)(vPos.y)
			, (int)(vScale.x)
			, (int)(vScale.y)
			, pTex->GetDC()
			, iCurCol * TILE_SIZE
			, iCurRow * TILE_SIZE
			, TILE_SIZE, TILE_SIZE
			, SRCCOPY);
	}
	else
	{
		Rectangle(_dc
			, (int)(vPos.x)
			, (int)(vPos.y)
			, (int)(vPos.x + vScale.x)
			, (int)(vPos.y + vScale.y));

		wchar_t strName[50] = {};
		
		wcscpy_s(strName, GetName().c_str());

		SetTextColor(_dc, RGB(0, 0, 0));   // 텍스트 색상 (검정)
		SetBkMode(_dc, TRANSPARENT);       // 배경 투명 설정
		TextOut(_dc, (int)(vPos.x + 5), (int)(vPos.y + 5), strName, wcslen(strName)); // 텍스트 출력 위치 (사각형의 좌측 상단)
	}
}