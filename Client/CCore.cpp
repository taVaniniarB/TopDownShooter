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
	, m_arrBrush{}
	, m_arrPen{}
	, m_arrFont{}
	, m_pMemTex(nullptr)
	, m_hMenu(0)
{
}

CCore::~CCore()
{// 프로그램 종료 시점>데이터 영역의 싱글턴 객체 삭제>소멸자 호출
	ReleaseDC(m_hWnd, m_hDC);

	for (int i = 0; i < (UINT)PEN_TYPE::END; ++i)
	{
		DeleteObject(m_arrPen[i]);
	}

	// 참고: DeleteMenu: 메뉴의 항목 지우기
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


	// 입력받은 해상도에 맞게 윈도우 크기 조절
	ChangeWindowSize(Vec2((float)_ptResolution.x, (float)_ptResolution.y), false);

	// 메뉴바 생성 (메모리 해제 필요)
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_CLIENT));

	m_hDC = GetDC(m_hWnd);

	// 이중 버퍼링 용도의 텍스처 한 장을 만든다.
	m_pMemTex = CResMgr::GetInst()->CreateTexture(L"BackBuffer", (UINT)m_ptResolution.x, (UINT)m_ptResolution.y);

	InitGDIPlus();

	// 자주 사용할 펜, 브러쉬 생성
	CreateBrushPen();


	CreateFonts();
	
	//Manager 초기화
	CPathMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CCamera::GetInst()->init();
	CSceneMgr::GetInst()->init();
	CSoundMgr::GetInst()->init();

	// Sound 로드
	CResMgr::GetInst()->LoadSound(L"Kid_Rock", L"sound\\BGM\\Kid_Rock.wav");
	CResMgr::GetInst()->LoadSound(L"M16", L"sound\\effect\\M16.wav");
	CResMgr::GetInst()->LoadSound(L"Shotgun", L"sound\\effect\\Shotgun.wav");
	CResMgr::GetInst()->LoadSound(L"Swing1", L"sound\\effect\\Swing1.wav");
	CResMgr::GetInst()->LoadSound(L"Cut1", L"sound\\effect\\Cut1.wav");
	CResMgr::GetInst()->LoadSound(L"Hit1", L"sound\\effect\\Hit1.wav");


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
	CTimeMgr::GetInst()->update(); // dt 값 계산
	CKeyMgr::GetInst()->update(); // 키 상태 체크
	CCamera::GetInst()->update();




	// UI 이벤트 체크
	CUIMgr::GetInst()->update();

	// ============
	// Scene Update
	// ============
	CSceneMgr::GetInst()->update(); // 현재 씬 돌리기

	// 충돌체크
	CCollisionMgr::GetInst()->update();



	// ==========
	// Rendering
	// ==========
	// 버퍼 텍스처의 DC에 그림을 그린다
	// 화면 Clear
	Clear();

	CSceneMgr::GetInst()->render(m_pMemTex->GetDC());

	// Scene 렌더 후 카메라 렌더
	CCamera::GetInst()->render(m_pMemTex->GetDC());

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y
		, m_pMemTex->GetDC(), 0, 0, SRCCOPY); // 메모리 DC의 것을 메인으로 복사
	CTimeMgr::GetInst()->render();

	// ================
	// 이벤트 지연 처리
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

// 브러시와 펜은 윈도우를 통해 얻을 수도 있겠으나, 코어를 통해 얻어오도록 하자!
void CCore::CreateBrushPen()
{
	// GetStockObject는 자주 사용되는 도구의 집합을 윈도우에서 지정해둔 것이며
	// 따로 삭제 신경 쓸 필요 없음
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);

	// red, green, blue pen
	// 직접 만든 거라 지워줘야 함
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
		60, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("System"));

	m_arrFont[(UINT)FONT_TYPE::GULIM] = CreateFont(
		20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("굴림"));
}


void CCore::DockMenu()
{
	// 툴씬에서 사용할 메뉴를 붙인다
	SetMenu(m_hWnd, m_hMenu);
	ChangeWindowSize(GetResolution(), true);
}

void CCore::DivideMenu()
{
	// nullptr: 메인 윈도우에서 메뉴를 뺀다는 뜻
	SetMenu(m_hWnd, nullptr);
	ChangeWindowSize(GetResolution(), false);
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	RECT rt = { 0, 0, (long)_vResolution.x, (long)_vResolution.y };
	// 사각형 사이즈, 가장 기본적인 옵션, 메뉴바 유무
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
}
