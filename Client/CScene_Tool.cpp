#include "pch.h"
#include "CScene_Tool.h"
#include "CKeyMgr.h"

#include "CSceneMgr.h"
#include "CCore.h"
#include "CCamera.h"
#include "CPathMgr.h"

#include "CTile.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CUI.h"
#include "CUIMgr.h"

#include "resource.h"
#include "CPanelUI.h"
#include "CBtnUI.h"


// 함수포인터 인자 전달해야 하는데 전역함수가 아래에 있어서 전방선언
void ChangeScene(DWORD_PTR, DWORD_PTR);


CScene_Tool::CScene_Tool()
	: m_pUI(nullptr)
	, m_pSelectedTileIdx(0)
{
}

CScene_Tool::~CScene_Tool()
{
}


void CScene_Tool::Enter()
{
	// 타일 생성
	CreateTile(5, 5);
	
	// UI 생성
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	CUI* pPanelUI = new CPanelUI;

	pPanelUI->SetName(L"ParentUI");
	pPanelUI->SetScale(Vec2(300.f, 200.f));
	pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x, 0.f));

	CBtnUI* pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"ChildUI");
	pBtnUI->SetScale(Vec2(100.f, 40.f));
	// 부모로부터 상대적 위치
	pBtnUI->SetPos(Vec2(0.f, 0.f));

	// 버튼 클릭시 세이브하도록 구현
	// 멤버함수 포인터의 경우 함수명 앞에 & 붙여줘야만 주소로 인식
	// BtnUI에서 부모 클래스의 함수를 받도록 해줬으므로 부모 오브젝트로 캐스팅 필요
	((CBtnUI*)pBtnUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveTileData);
	
	pPanelUI->AddChild(pBtnUI);

	// pUI 하나만 씬에 넣어두면, 계층적으로 자식을 호출
	AddObject(pPanelUI, GROUP_TYPE::UI);


    // 과제 - 타일 UI
	CUI* pTilePanelUI = new CPanelUI;
	pTilePanelUI->SetName(L"TileParentUI");
	pTilePanelUI->SetScale(Vec2(300.f, 400.f));
	pTilePanelUI->SetPos(Vec2(vResolution.x - pTilePanelUI->GetScale().x, 100.f));
	AddObject(pTilePanelUI, GROUP_TYPE::UI);

	for (UINT i = 0; i < 12; ++i)
	{
		const int iMaxRow = 3;
		int iRow = 0; int iCol = 0;

		wchar_t name[20] = L"ChildUI";
		wchar_t idx[10];
		_itow_s(i, idx, 10);
		wcscat_s(name, idx);

		CBtnUI* pTileSelectUI = new CBtnUI;
		pTileSelectUI->SetName(name); //두 wchar 버퍼 합치기 함수로 타일에 이름을 부여

		int iXScale = (int)(pTilePanelUI->GetScale().x / iMaxRow);
		int iTileUIScale = 80;
		
		pTileSelectUI->SetScale(Vec2((float)iTileUIScale, (float)iTileUIScale));
		iRow = i / iMaxRow;
		iCol = (int)(i % iMaxRow);
		pTileSelectUI->SetPos(Vec2((float)iCol * iXScale, (float)iRow * iXScale));

		((CBtnUI*)pTileSelectUI)->SetClickedCallBack(this, (SCENE_MEMFUNC_INT)&CScene_Tool::SetSelectedTile, i);
		pTilePanelUI->AddChild(pTileSelectUI);
	}


	// 복사본 UI
	/* 복사본 UI
	CUI* pClonePanel = pPanelUI->Clone();
	pClonePanel->SetPos(pClonePanel->GetPos() + Vec2(-100.f, 100.f));
	
	// 복제한 UI의 버튼에 콜백함수 등록
	((CBtnUI*)pClonePanel->GetChildUI()[0])->SetClickedCallBack(&ChangeScene, 0, 0);
	
	AddObject(pClonePanel, GROUP_TYPE::UI);

	m_pUI = pClonePanel;

	*/

	// 카메라 LookAt 지정
	// 	   //위의 UI 생성에서 해상도 받아오는 부분 있어서, 일단 주석 처리
	//Vec2 vResolution = CCore::GetInst()->GetResolution();
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

}

void CScene_Tool::Exit()
{
	DeleteAll();
}




void CScene_Tool::update()
{
	CScene::update();

	// 마우스 좌표 받아서 타일 인덱스를 변환
	SetTileIdx();

	// G키를 누르면 툴씬이 기억하고 있는 UI로 강제 포커싱
	if (KEY_TAP(KEY::G))
	{
		CUIMgr::GetInst()->SetFocusedUI(m_pUI);
	}

	// 컨트롤 + S 누르면 타일을 저장
	if (KEY_TAP(KEY::S) && KEY_HOLD(KEY::CTRL))
	{
		SaveTileData();
	}

	// F 누르면 파일 로드
	if (KEY_TAP(KEY::F))
	{
		LoadTileData();
	}

	if (KEY_HOLD(KEY::LBTN) || KEY_TAP(KEY::LBTN))
	{
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);
		
		int iTileX = GetTileX();
		int iTileY = GetTileY();
		if (iTileX)

		// 클릭된 타일의 인덱스를 변경
		ChangeTile(vMousePos, m_pSelectedTileIdx);
	}
}


void CScene_Tool::SetTileIdx()
{
	if (KEY_TAP(KEY::LBTN))
	{
		// 마우스 위치를 실제좌표로 변환
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);

		int iTileX = (int)GetTileX();
		int iTileY = (int)GetTileY();

		// 음수좌표 고려하여 int
		int iCol = (int)vMousePos.x / TILE_SIZE;
		int iRow = (int)vMousePos.y / TILE_SIZE;

		// 타일 없는 곳 클릭 예외처리
		if (vMousePos.x < 0.f || iCol >= iTileX
			|| vMousePos.y < 0.f || iRow >= iTileY)
		{
			return;
		}


		UINT iIdx = iRow * iTileX + iCol;

		const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
		// CObject 포인터이기 때문에, CTile 타입으로 캐스팅
		// 뒤에서부터 읽음을 유념하여 괄호 쳐주기
		((CTile*)vecTile[iIdx])->AddImgIdx();
	}
	// 마우스 좌표가 가리킨 타일이 어떤 타일인지 알아내기
	// 마우스 좌표.x, y를 각각 타일 사이즈로 나누면 몇 번째 행, 열인지 알 수 있다
	// 이렇게 구한 행, 열과, 타일 생성 시 입력한 최대 행, 열 값을 가지고 오브젝트의 타일 그룹 벡터를 조회
	// 벡터 인덱스를 통해 반복문 안 쓰고 바로 접근 가능
	
}

void CScene_Tool::SaveTileData()
{
	// 창을 띄워서 어디에 뭐라 저장할지 정하고
	// 그 경로 받아와서 SaveTile 함수를 호출하자.

	OPENFILENAME ofn = {};

	// 저장될 파일의 절대경로를 넣을 변수
	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();

	// 완성된 경로가 채워질 주소(배열의 시작주소)를 가리킨다
	ofn.lpstrFile = szName;

	ofn.nMaxFile = sizeof(szName); // 버퍼 크기 (바이트)
	// lpstrFilter: 확장자 필터 규칙
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	// 필터 인덱스: 골라놓은 것 중 초기 필터를 무엇으로 세팅하는가?
	// 0선택: ALL이라는 뜻...
	ofn.nFilterIndex = 0;
	//창 오픈 시 캡션인데, 수정 가능한 값 줘야해서 일단 패스
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";
	// 저장창의 초기 경로
	ofn.lpstrInitialDir = strTileFolder.c_str();
	// 패스가 존재하고 파일이 존재해야 한다 (비트연산 조합)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	// 세팅한 정보 바탕으로 창 열기
	if (GetSaveFileName(&ofn)) 
	{
		SaveTile(szName);
	}
}

void CScene_Tool::SaveTile(const wstring& _strFilePath)
{
	// 커널 오브젝트
	FILE* pFile = nullptr;

	// C++에서 기본으로 제공하는 입출력 함수
	// 이중 포인터를 인자로 넘김으로써, 실행 후에 pFile은 무언가 가리키게 됨
	// 파일포인터의 주소 / 절대경로(wchar*) / 모드(w: 쓰기)
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");

	// 파일 열기(fopen) 실패. 성공했다면 어떤 주소가 채워져 있을테니...
	assert(pFile);

	// 데이터 저장
	// 저장할 것: 타일 가로세로 개수
	UINT xCount = GetTileX();
	UINT yCount = GetTileY();

	// 입력할 데이터의 시작주소 / 입력할 데이터의 크기 / 개수(배열일 경우 배열의크기) / 
	// 시작주소가 void포인터인데, 어떤 데이터든 받을 수 있게 하기 위함임.
	// 일단 받은 다음, 두 번째 인자 크기 단위로 읽는다.
	// 저장할 데이터가 무엇이든 간에 범용적 적용을 위함.
	fwrite(&xCount, sizeof(UINT), 1, pFile);
	fwrite(&yCount, sizeof(UINT), 1, pFile);


	// 타일 각각의 정보는 각자 자기(타일) 쪽에 구현 (세이브가 아닌 타일의 역할)
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);


	// 모든 타일을 개별적으로, 저장할 데이터 저장하게 함
	// 타일 전체를 순회하며 각자 타일에게 저장할거있음 저장해라, save 함수 호출
	// 타일들은 각각 자신을 save 한다
	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Save(pFile);
	}


	// 이 파일에 대한 파일 입출력 닫기
	fclose(pFile);

}

//어떤 파일을 열지 UI 통해 지정하고, 그것을 Load하기
void CScene_Tool::LoadTileData()
{
	// 창 열어서 경로를 알아내고 load tile 해야한다.

	OPENFILENAME ofn = {};

	// 오픈할 파일의 절대경로를 넣을 변수
	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();

	// 선택한 파일의 경로가 채워질 주소(배열의 시작주소)를 가리킨다
	ofn.lpstrFile = szName;

	ofn.nMaxFile = sizeof(szName); // 버퍼 크기 (바이트)
	// lpstrFilter: 확장자 필터 규칙....?
	// 모든 파일: L"ALL\0*.*"
	// 저장 창 열렸을 때 필더된 확장자 파일들만 목록에 뜨는거!!!!
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	// 필터 인덱스: 골라놓은 것 중 초기 필터를 무엇으로 세팅하는가?
	// 0선택: ALL이라는 뜻...
	ofn.nFilterIndex = 0;
	//창 오픈 시 캡션인데, 수정 가능한 값 줘야해서 일단 패스
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";
	// 저장창의 초기 경로
	ofn.lpstrInitialDir = strTileFolder.c_str();
	// 패스가 존재하고 파일이 존재해야 한다 (비트연산 조합)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	// 세팅한 정보 바탕으로 오픈 창 열기
	if (GetOpenFileName(&ofn))
	{
		// 상대경로 변환
		wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(szName);
		
		LoadTile(strRelativePath);
	}
}

void CScene_Tool::SetSelectedTile(int _idx)
{
	m_pSelectedTileIdx = _idx;
}


// 전역 함수
void ChangeScene(DWORD_PTR, DWORD_PTR)
{
	ChangeScene(SCENE_TYPE::START);
}




// Tool Scene의 멤버 X, 보기 편하게 배치만 이렇게 해둔거임
// =======================
// Tile Count Window Proc
// =======================
INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			// dialog 안의 데이터 가져오게 해주는 함수
			// 핸들, ID(ID 쓰려면 리소스 헤더 참조해주기), 필요없음, 음수처리)
			UINT iXCount = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
			UINT iYCount = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);

			// 여기는 멤버함수가 아니므로 SceneMgr에서부터 scene 받아와야 함
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			// 무조건 Tool Scene에서만 쓸 UI 기능이므로, 메뉴바는 ToolScene에서만 활성화한다
			// 그리고 다운캐스팅을 통해 툴 씬인지 체크함으로써 코드 상에서도 메뉴바는 툴 씬에서 사용할 기능임을 명시적으로 표현한다

			// ToolScene 확인
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);


			pToolScene->CreateTile(iXCount, iYCount);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}