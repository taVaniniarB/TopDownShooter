#include "pch.h"
#include "CScene_Tool.h"
#include "CKeyMgr.h"

#include "CSceneMgr.h"
#include "CCore.h"
#include "CCamera.h"

#include "CTile.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CUI.h"

#include "resource.h"


CScene_Tool::CScene_Tool()
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

	CUI* pUI = new CUI;
	pUI->SetScale(Vec2(100.f, 30.f));
	pUI->SetPos(Vec2(vResolution.x - pUI->GetScale().x, 0.f));
	
	AddObject(pUI, GROUP_TYPE::UI);

	// 카메라 LookAt 지정
	// 	   //위의 UI 생성에서 해상도 받아오는 부분 있어서, 일단 주석 처리
	//Vec2 vResolution = CCore::GetInst()->GetResolution();
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

}

void CScene_Tool::Exit()
{
}




void CScene_Tool::update()
{
	CScene::update();

	SetTileIdx();
}


void CScene_Tool::SetTileIdx()
{
	if (KEY_TAP(KEY::LBTN))
	{
		// 마우스 위치를 실제좌표로 변환
		Vec2 vMousePos = MOUSE_POS;;
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

			// 타일 생성 전에 해야할 것: 이전 타일 지우기
			// 타일 그룹 오브젝트들을 모두 날리자
			// 여기는 멤버함수가 아니므로 SceneMgr에서부터 scene 받아와야 함
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			// 무조건 Tool Scene에서만 쓸 UI 기능이므로, 메뉴바는 ToolScene에서만 활성화한다
			// 그리고 다운캐스팅을 통해 툴 씬인지 체크함으로써 코드 상에서도 메뉴바는 툴 씬에서 사용할 기능임을 명시적으로 표현한다

			// ToolScene 확인
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);

			pToolScene->DeleteGroup(GROUP_TYPE::TILE);
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