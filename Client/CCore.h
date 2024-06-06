#pragma once

class CCore
{
	SINGLE(CCore);
private:
	HWND	m_hWnd;			// 메인 윈도우 핸들
	POINT	m_ptResolution; // 메인 윈도우 해상도
	HDC		m_hDC;			// 메인 윈도우에 Draw 할 DC

	HBITMAP m_hBit;			// 버퍼 비트맵
	HDC		m_memDC;		// 버퍼에 Draw 할 DC

	// 자주 사용하는 GDI Object
	// 필요할 때마다 생성은 비효율적이니까
	HBRUSH	m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];;

private:
	void CreateBrushPen();

public: //코어 클래스의 초기화 함수를 만들자
	//실패 체크 위해 정수 값 반환하자
	int init(HWND _hWnd, POINT _ptResolution);
	void progress(); //메시지가 없는 동안의 코어 클래스의 작업
	
public:
	HWND GetMainHwnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }
	POINT GetResolution() { return m_ptResolution; }
	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }
};



