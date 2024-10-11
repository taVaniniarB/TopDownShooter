#include "pch.h"
#include "CCore.h"

#include "CObject.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CCamera.h"
#include "CUIMgr.h"

#include "CResMgr.h"
#include "CTexture.h"
#include "SelectGDI.h"

#include "resource.h"

#include "CSoundMgr.h"
#include "CSound.h"


CCore::CCore()
	: m_hWnd(0)
	, m_ptResolution{}
	, m_hDC(0)
	//, m_hBit(0)
	//, m_memDC(0)
	, m_arrBrush{}
	, m_arrPen{}
	, m_arrFont{}
	, m_pMemTex(nullptr)
	, m_hMenu(0)
{
}

CCore::~CCore()
{// ���α׷� ���� ����>������ ������ �̱��� ��ü ����>�Ҹ��� ȣ��
	ReleaseDC(m_hWnd, m_hDC);

	for (int i = 0; i < (UINT)PEN_TYPE::END; ++i)
	{
		DeleteObject(m_arrPen[i]);
	}

	// ����: DeleteMenu: �޴��� �׸� �����
	DestroyMenu(m_hMenu);

	for (int i = 0; i < (UINT)FONT_TYPE::END; ++i)
	{
		RemoveFontMemResourceEx(m_arrFont[i]);
	}
}



int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;


	// �Է¹��� �ػ󵵿� �°� ������ ũ�� ����
	ChangeWindowSize(Vec2((float)_ptResolution.x, (float)_ptResolution.y), false);

	// �޴��� ���� (�޸� ���� �ʿ�)
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_CLIENT));

	m_hDC = GetDC(m_hWnd);

	// ���� ���۸� �뵵�� �ؽ�ó �� ���� �����.
	m_pMemTex = CResMgr::GetInst()->CreateTexture(L"BackBuffer", (UINT)m_ptResolution.x, (UINT)m_ptResolution.y);

	InitGDIPlus();
	// ���� ���۸� �뵵�� ��Ʈ�ʰ� DC�� ����� (����� �ؽ�ó�� ��ġ�Ƿ� ���� ������� ����)
	//m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	//m_memDC = CreateCompatibleDC(m_hDC);
	//HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	//DeleteObject(hOldBit);

	// ���� ����� ��, �귯�� ����
	CreateBrushPen();


	CreateFonts();
	
	//Manager �ʱ�ȭ
	CPathMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CCamera::GetInst()->init();
	CSceneMgr::GetInst()->init();
	CSoundMgr::GetInst()->init();

	// Sound �ε� �׽�Ʈ
	CResMgr::GetInst()->LoadSound(L"Kid_Rock", L"sound\\BGM\\Kid_Rock.wav");
	CResMgr::GetInst()->LoadSound(L"M16", L"sound\\effect\\M16.wav");
	CResMgr::GetInst()->LoadSound(L"Shotgun", L"sound\\effect\\Shotgun.wav");


	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Kid_Rock");

	pNewSound->Play();

	//pNewSound->SetPosition(50.f);
	pNewSound->PlayAsBGM(true);
	pNewSound->SetVolume(10.f);


	return S_OK;
}



void CCore::progress()
{
	// ==============
	// Manager Update
	// ==============
	CTimeMgr::GetInst()->update(); // dt �� ���
	CKeyMgr::GetInst()->update(); // Ű ���� üũ
	CCamera::GetInst()->update();




	// UI �̺�Ʈ üũ
	CUIMgr::GetInst()->update();

	// ============
	// Scene Update
	// ============
	CSceneMgr::GetInst()->update(); // ���� �� ������

	// �浹üũ
	CCollisionMgr::GetInst()->update();



	// ==========
	// Rendering
	// ==========
	// ���� �ؽ�ó�� DC�� �׸��� �׸���
	// ȭ�� Clear
	Clear();

	CSceneMgr::GetInst()->render(m_pMemTex->GetDC());

	// Scene ���� �� ī�޶� ����
	CCamera::GetInst()->render(m_pMemTex->GetDC());

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y
		, m_pMemTex->GetDC(), 0, 0, SRCCOPY); // �޸� DC�� ���� �������� ����
	CTimeMgr::GetInst()->render();

	// ================
	// �̺�Ʈ ���� ó��
	// ================
	CEventMgr::GetInst()->update();
}

void CCore::InitGDIPlus()
{
	ULONG_PTR gdiplusToken = 0;
	Gdiplus::GdiplusStartupInput gdistartupinput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdistartupinput, nullptr);
}

void CCore::Clear()
{
	SelectGDI gdi(m_pMemTex->GetDC(), BRUSH_TYPE::BLACK);
	Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
}

// �귯�ÿ� ���� �����츦 ���� ���� ���� �ְ�����, �ھ ���� �������� ����!
void CCore::CreateBrushPen()
{
	// GetStockObject�� ���� ���Ǵ� ������ ������ �����쿡�� �����ص� ���̸�
	// ���� ���� �Ű� �� �ʿ� ����
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);

	// red, green, blue pen
	// ���� ���� �Ŷ� ������� ��
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_arrPen[(UINT)PEN_TYPE::YELLOW] = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
}

void CCore::CreateFonts()
{
	m_arrFont[(UINT)FONT_TYPE::IMPACT] = CreateFont(
		100, 10, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Impact"));

	m_arrFont[(UINT)FONT_TYPE::SYSTEM] = CreateFont(
		70, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("System"));

	m_arrFont[(UINT)FONT_TYPE::GULIM] = CreateFont(
		25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("����"));

}


void CCore::DockMenu()
{
	// �������� ����� �޴��� ���δ�
	SetMenu(m_hWnd, m_hMenu);
	ChangeWindowSize(GetResolution(), true);
}

void CCore::DivideMenu()
{
	// nullptr: ���� �����쿡�� �޴��� ���ٴ� ��
	SetMenu(m_hWnd, nullptr);
	ChangeWindowSize(GetResolution(), false);
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	RECT rt = { 0, 0, (long)_vResolution.x, (long)_vResolution.y };
	// �簢�� ������, ���� �⺻���� �ɼ�, �޴��� ����
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
}
