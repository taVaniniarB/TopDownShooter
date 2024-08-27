#pragma once

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
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];;

private:
	void CreateBrushPen();

public: //�ھ� Ŭ������ �ʱ�ȭ �Լ��� ������
	//���� üũ ���� ���� �� ��ȯ����
	int init(HWND _hWnd, POINT _ptResolution);
	void progress(); //�޽����� ���� ������ �ھ� Ŭ������ �۾�
	
public:
	HWND GetMainHwnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }
	POINT GetResolution() { return m_ptResolution; }
	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }
};



