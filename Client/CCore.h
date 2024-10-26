#pragma once
#include <wingdi.h>

class CTexture;

class CCore
{
	SINGLE(CCore);
private:
	HWND	m_hWnd;			// ���� ������ �ڵ�
	POINT	m_ptResolution; // ���� ������ �ػ�
	HDC		m_hDC;			// ���� �����쿡 Draw �� DC


	// ���� ���۸� �뵵�� ����۸� �ؽ�ó�� ��ü�� (����� ���ļ�)
	//HBITMAP m_hBit;			// ���� ��Ʈ��
	//HDC		m_memDC;		// ���ۿ� Draw �� DC (�޸� DC)

	CTexture* m_pMemTex;		// ����� �ؽ�ó

	// ���� ����ϴ� GDI Object
	// �ʿ��� ������ ������ ��ȿ�����̴ϱ�
	HBRUSH	m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];
	HFONT	m_arrFont[(UINT)FONT_TYPE::END];

	// �޴�
	HMENU m_hMenu; // Tool Scene ������ ����ϱ� ���� �޴�

private:
	void Clear();
	void CreateBrushPen();
	void CreateFonts();

public: //�ھ� Ŭ������ �ʱ�ȭ �Լ��� ������
	//���� üũ ���� ���� �� ��ȯ����
	int init(HWND _hWnd, POINT _ptResolution);
	void progress(); //�޽����� ���� ������ �ھ� Ŭ������ �۾�

private:
	void InitGDIPlus();
	
public:
	void DockMenu();
	void DivideMenu();
	void ChangeWindowSize(Vec2 _vResolution, bool _bMenu);

public:
	HWND GetMainHwnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }

	POINT GetResolution() { return m_ptResolution; }
	float GetWidth() { return (float)m_ptResolution.x; }
	float GetHeight() { return (float)m_ptResolution.y; }
	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }
	HFONT GetFont(FONT_TYPE _eType) { return m_arrFont[(UINT)_eType]; }
};



