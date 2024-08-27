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
	// 파일로부터 로딩한 데이터를 비트맵으로 생성
	// 필요없음 / 파일명 / 비트맵 형태로 불러오겠다 / 얼마 크기만큼 불러올래? (0-전체) / 로드 옵션(가장 기본적인 것)
	m_hBit = (HBITMAP)LoadImage(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	assert(m_hBit);

	// 비트맵과 연결할 텍스처 자체 dc
	// 메인 DC와 호환되는 텍스처 DC를 생성
	m_dc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
	
	// 비트맵과 DC 연결
	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(hPrevBit);
	

	// 가로세로 길이

	// 특정 오브젝트에 대한 정보를 주는 함수
	// 비트맵 정보 멤버에 정보를 저장
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}

void CTexture::Create(UINT _iwidth, UINT _iHeight)
{
	// 메인DC 가져와서, 그것을 기반으로 비트맵과 DC를 만들고, 둘을 연결해준다.

	HDC mainDC = CCore::GetInst()->GetMainDC();
	m_hBit = CreateCompatibleBitmap(mainDC, _iwidth, _iHeight);
	m_dc = CreateCompatibleDC(mainDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(hOldBit);

	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}
