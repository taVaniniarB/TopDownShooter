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
	UINT iWidth = pTex->Width();
	UINT iHeight = pTex->Height();

	// Ÿ�� ���� ���̿� ���� �ִ� �� ����
	UINT iMaxCol = iWidth / TILE_SIZE;

	//�� ����(���� ����: �ε��� �ʰ� ����ó��)
	UINT iMaxRow = iHeight / TILE_SIZE;

	UINT iCurRow = (UINT)m_iIdx / iMaxCol;
	UINT iCurCol = (UINT)m_iIdx % iMaxCol;

	// �̹��� ������ �Ѿ �ε���
	if (iMaxCol <= iCurCol)
		assert(nullptr);

	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();


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

	if (m_bSelected)
	{
		SelectGDI select(_dc, PEN_TYPE::GREEN);
		Rectangle(_dc
			, (int)(vPos.x)
			, (int)(vPos.y)
			, (int)(vPos.x + vScale.x)
			, (int)(vPos.y + vScale.y));
	}
}