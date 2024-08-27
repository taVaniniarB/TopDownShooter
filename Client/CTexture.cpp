#include "pch.h"
#include "CTexture.h"
#include "CCore.h"

CTexture::CTexture()
	: m_hBit(0)
	, m_dc(0)
	, m_bitInfo{}
{
}

CTexture::~CTexture()
{
	DeleteDC(m_dc);
	DeleteObject(m_hBit);
}

void CTexture::Load(const wstring& _strFilePath)
{
	// ���Ϸκ��� �ε��� �����͸� ��Ʈ������ ����
	// �ʿ���� / ���ϸ� / ��Ʈ�� ���·� �ҷ����ڴ� / �� ũ�⸸ŭ �ҷ��÷�? (0-��ü) / �ε� �ɼ�(���� �⺻���� ��)
	m_hBit = (HBITMAP)LoadImage(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	assert(m_hBit);

	// ��Ʈ�ʰ� ������ �ؽ�ó ��ü dc
	// ���� DC�� ȣȯ�Ǵ� �ؽ�ó DC�� ����
	m_dc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
	
	// ��Ʈ�ʰ� DC ����
	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(hPrevBit);
	

	// ���μ��� ����

	// Ư�� ������Ʈ�� ���� ������ �ִ� �Լ�
	// ��Ʈ�� ���� ����� ������ ����
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}

void CTexture::Create(UINT _iwidth, UINT _iHeight)
{
	// ����DC �����ͼ�, �װ��� ������� ��Ʈ�ʰ� DC�� �����, ���� �������ش�.

	HDC mainDC = CCore::GetInst()->GetMainDC();
	m_hBit = CreateCompatibleBitmap(mainDC, _iwidth, _iHeight);
	m_dc = CreateCompatibleDC(mainDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(hOldBit);

	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}
