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
#include "CTileBtnUI.h"

#include "CResMgr.h"
#include "CTexture.h"
#include "CWall.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CSceneChanger.h"

#include "SelectGDI.h"

// 함수포인터 인자 전달해야 하는데 전역함수가 아래에 있어서 전방선언
void ChangeScene(DWORD_PTR, DWORD_PTR);


CScene_Tool::CScene_Tool()
	: m_pUI(nullptr)
	, m_iSelectedTileIdx(-1)
	, m_eSelctedObj(SELECT_OPTION::NONE)
	, m_eSelectedWall(WALL_TYPE::NONE)
	, m_eSelectedWallDir(WALL_DIR::TOP)
	, m_eSelectedWallDir2(WALL_DIR::END)
	, m_bGrid(true)
{
}

CScene_Tool::~CScene_Tool()
{
}


void CScene_Tool::Enter()
{
	CCamera::GetInst()->SetTarget(nullptr);

	// 툴씬에서 사용할 메뉴 추가
	CCore::GetInst()->DockMenu();

	// 타일 생성
	CreateTile(50, 30);

	// UI 생성
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	//CUI* pPanelUI = new CPanelUI;
	//pPanelUI->SetName(L"ParentUI");
	//pPanelUI->SetScale(Vec2(300.f, 200.f));
	//pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x, 0.f));
	//CBtnUI* pBtnUI = new CBtnUI;
	//pBtnUI->SetName(L"ChildUI");
	//pBtnUI->SetScale(Vec2(100.f, 40.f));
	//// 부모로부터 상대적 위치
	//pBtnUI->SetPos(Vec2(0.f, 0.f));
	//// 버튼 클릭시 세이브하도록 구현
	//// 멤버함수 포인터의 경우 함수명 앞에 & 붙여줘야만 주소로 인식
	//// BtnUI에서 부모 클래스의 함수를 받도록 해줬으므로 부모 오브젝트로 캐스팅 필요
	//((CBtnUI*)pBtnUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveTileData);
	//pPanelUI->AddChild(pBtnUI);
	//// pUI 하나만 씬에 넣어두면, 계층적으로 자식을 호출
	//AddObject(pPanelUI, GROUP_TYPE::UI);


	// 타일 UI
	CUI* pTilePanelUI = new CPanelUI;
	pTilePanelUI->SetName(L"TileParentUI");
	pTilePanelUI->SetScale(Vec2(200.f, 400.f));
	pTilePanelUI->SetPos(Vec2(vResolution.x - pTilePanelUI->GetScale().x, 100.f));
	AddObject(pTilePanelUI, GROUP_TYPE::UI);

	// 타일 자식 UI
	for (UINT i = 0; i < 4; ++i)
	{
		const int iMaxRow = 4;
		int iRow = 0; int iCol = 0;

		CTileBtnUI* pTileSelectUI = new CTileBtnUI;

		//int iXScale = (int)(pTilePanelUI->GetScale().x / iMaxRow);

		pTileSelectUI->SetScale(Vec2(50.f, 50.f));
		iRow = i / iMaxRow;
		iCol = (int)(i % iMaxRow);
		pTileSelectUI->SetPos(Vec2((float)iCol * 50.f, (float)iRow * 50.f));

		((CTileBtnUI*)pTileSelectUI)->SetClickedCallBack(this, (SCENE_MEMFUNC_INT)&CScene_Tool::SetSelectedTile, i);
		pTilePanelUI->AddChild(pTileSelectUI);

		// 타일 UI의 텍스처
		CTexture* pTex = CResMgr::GetInst()->FindTexture(L"Tile");
		pTileSelectUI->SetTexture(pTex);
		pTileSelectUI->SetIdx(i);
	}
	// 타일벽 버튼
	for (UINT i = 4; i < 8; ++i)
	{
		const int iMaxRow = 4;
		int iRow = 0; int iCol = 0;

		CTileBtnUI* pTileSelectUI = new CTileBtnUI;

		int iXScale = (int)(pTilePanelUI->GetScale().x / iMaxRow);

		pTileSelectUI->SetScale(Vec2(50.f, 50.f));
		iRow = i / iMaxRow;
		iCol = (int)(i % iMaxRow);
		pTileSelectUI->SetPos(Vec2((float)iCol * iXScale, (float)iRow * iXScale));

		((CTileBtnUI*)pTileSelectUI)->SetClickedCallBack(this, (SCENE_MEMFUNC_INT)&CScene_Tool::SetSelectedTileWall, i);
		pTilePanelUI->AddChild(pTileSelectUI);

		// 타일 UI의 텍스처
		CTexture* pTex = CResMgr::GetInst()->FindTexture(L"Tile");
		pTileSelectUI->SetTexture(pTex);
		pTileSelectUI->SetIdx(i);
	}
	// 벽 버튼
	{
		CTileBtnUI* pTileSelectUI = new CTileBtnUI;
		pTileSelectUI->SetScale(Vec2(50.f, 50.f));
		pTileSelectUI->SetPos(Vec2(0.f, 170.f));
		pTileSelectUI->SetName(L"Wall");
		((CTileBtnUI*)pTileSelectUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SetSelectedWall);
		pTilePanelUI->AddChild(pTileSelectUI);
	}
	// 모서리 버튼
	{
		CTileBtnUI* pTileSelectUI = new CTileBtnUI;
		pTileSelectUI->SetScale(Vec2(50.f, 50.f));
		pTileSelectUI->SetPos(Vec2(50.f, 170.f));
		pTileSelectUI->SetName(L"Corner");
		((CTileBtnUI*)pTileSelectUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SetSelectedCorner);
		pTilePanelUI->AddChild(pTileSelectUI);
	}

	// 플레이어 버튼
	{
		CBtnUI* pPlayerSelectUI = new CBtnUI;
		pPlayerSelectUI->SetScale(Vec2(50.f, 50.f));
		pPlayerSelectUI->SetPos(Vec2(0.f, 220.f));
		pPlayerSelectUI->SetName(L"Player");
		((CBtnUI*)pPlayerSelectUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SetSelectedPlayer);
		pTilePanelUI->AddChild(pPlayerSelectUI);
	}

	// 몬스터 버튼
	{
		CBtnUI* pMonSelectUI = new CBtnUI;
		pMonSelectUI->SetScale(Vec2(50.f, 50.f));
		pMonSelectUI->SetPos(Vec2(50.f, 220.f));
		pMonSelectUI->SetName(L"Monster");
		((CBtnUI*)pMonSelectUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SetSelectedMosnter);
		pTilePanelUI->AddChild(pMonSelectUI);
	}

	// SceneChanger 버튼
	{
		CBtnUI* pSCSelectUI = new CBtnUI;
		pSCSelectUI->SetScale(Vec2(50.f, 50.f));
		pSCSelectUI->SetPos(Vec2(50.f, 280.f));
		pSCSelectUI->SetName(L"Monster");
		((CBtnUI*)pSCSelectUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SetSelectedSC);
		pTilePanelUI->AddChild(pSCSelectUI);
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
	CCamera::GetInst()->FadeOut(1.f);
	// 화면이 잠깐 멈춰야할듯...?
	CCore::GetInst()->DivideMenu();
	DeleteAll();
}




void CScene_Tool::update()
{
	CScene::update();

	// G키를 누르면 툴씬이 기억하고 있는 UI로 강제 포커싱
	/*
	if (KEY_TAP(KEY::G))
	{
		CUIMgr::GetInst()->SetFocusedUI(m_pUI);
	}
	*/

	// 그리드 On/Off
	if (KEY_TAP(KEY::G))
	{
		m_bGrid = !m_bGrid;
	}

	// 컨트롤 + S 누르면 타일을 저장
	if (KEY_TAP(KEY::S) && KEY_HOLD(KEY::CTRL))
	{
		SaveSceneData();
	}

	// F 누르면 파일 로드
	if (KEY_TAP(KEY::F))
	{
		LoadSceneData();
	}

	if (!GetUIClicked() && !(KEY_NONE(KEY::LBTN)))
	{
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);

		switch (m_eSelctedObj)
		{
		case SELECT_OPTION::TILE:
		{
			ChangeTile(vMousePos, m_iSelectedTileIdx);
			if (KEY_HOLD(KEY::LSHIFT))
				ChangeTile(vMousePos, -1);
		}
		break;
		case SELECT_OPTION::WALL:
		{
			// shift + 클릭: 삭제
			if (KEY_HOLD(KEY::LSHIFT) && KEY_TAP(KEY::LBTN))
			{
				CScene::DeleteWall(vMousePos, m_eSelectedWall);
				break;
			}
			// 클릭: wsad + 클릭: 해당 위치의 벽 생성
			if (KEY_HOLD(KEY::LBTN))
			{
				if (KEY_HOLD(KEY::W))
				{
					m_eSelectedWallDir = WALL_DIR::TOP;
				}
				if (KEY_HOLD(KEY::A))
				{
					m_eSelectedWallDir = WALL_DIR::LEFT;
				}
				if (KEY_HOLD(KEY::S))
				{
					m_eSelectedWallDir = WALL_DIR::BOTTOM;
				}
				if (KEY_HOLD(KEY::D))
				{
					m_eSelectedWallDir = WALL_DIR::RIGHT;
				}

				if (m_eSelectedWallDir != WALL_DIR::END)
				{
					if (KEY_HOLD(KEY::W) && m_eSelectedWallDir != WALL_DIR::TOP)
					{
						m_eSelectedWallDir2 = WALL_DIR::TOP;
					}
					if (KEY_HOLD(KEY::A) && m_eSelectedWallDir != WALL_DIR::LEFT)
					{
						m_eSelectedWallDir2 = WALL_DIR::LEFT;
					}
					if (KEY_HOLD(KEY::S) && m_eSelectedWallDir != WALL_DIR::BOTTOM)
					{
						m_eSelectedWallDir2 = WALL_DIR::BOTTOM;
					}
					if (KEY_HOLD(KEY::D) && m_eSelectedWallDir != WALL_DIR::RIGHT)
					{
						m_eSelectedWallDir2 = WALL_DIR::RIGHT;
					}
				}


				if (m_eSelectedWallDir2 != WALL_DIR::END)
				{
					CScene::CreateWall(vMousePos, m_eSelectedWallDir, m_eSelectedWallDir2);
					m_eSelectedWallDir2 = WALL_DIR::END;
				}
				else
					CScene::CreateWall(vMousePos, m_eSelectedWallDir, m_eSelectedWall, m_iSelectedTileIdx);
				break;
			}

		}
		break;
		case SELECT_OPTION::PLAYER:
		{
			if (KEY_TAP(KEY::LBTN))
			{
				const vector<CObject*>& playerVec = GetGroupObject(GROUP_TYPE::PLAYER);
				if (playerVec.size() >= 1)
				{
					// 기존의 player를 삭제
					DeleteObject(playerVec[0]);
				}
				CScene::SpawnPlayer(vMousePos);
			}
		}
		break;
		case SELECT_OPTION::MONSTER:
		{
			if (KEY_TAP(KEY::LBTN))
			{
				// shift + 클릭: 삭제
				if (KEY_HOLD(KEY::LSHIFT))
				{
					const vector<CObject*>& monsterVec = GetGroupObject(GROUP_TYPE::MONSTER);
					
					for (size_t i = 0; i < monsterVec.size(); ++i)
					{
						Vec2 vMonScale = monsterVec[i]->GetScale();
						Vec2 vMonPos = monsterVec[i]->GetPos();
						
						if (vMousePos.x >= vMonPos.x - vMonScale.x / 2
							&& vMousePos.x <= vMonPos.x + vMonScale.x / 2
							&& vMousePos.y >= vMonPos.y - vMonScale.y / 2
							&& vMousePos.y <= vMonPos.y + vMonScale.y / 2)
						{
							DeleteObject(monsterVec[i]);
							return;
						}
					}
				}
				CScene::SpawnMonster(vMousePos);
			}
		}
		break;
		case SELECT_OPTION::SCENE_CHANGER:
		{
			SCENE_TYPE eScene = SCENE_TYPE::STAGE_02;
			
			if (KEY_TAP(KEY::LBTN))
			{
				const vector<CObject*>& sceneChangerVec = GetGroupObject(GROUP_TYPE::SCENE_CHANGER);
				if (sceneChangerVec.size() >= 1)
				{
					// 기존의 scene changer 삭제
					DeleteObject(sceneChangerVec[0]);
				}

				// 시작점 지정
				m_vStartPos = vMousePos;
			}
			if (KEY_HOLD(KEY::LBTN))
			{
				m_bDrawingSqare = true;

				// 마우스 드래그로 SceneChanger의 Position과 Scale을 정하자.
				
				Vec2 vLT, vRB;
				
				// 현재 마우스 위치 -> Start 위치 향하는 벡터
				Vec2 vPosDiff = m_vStartPos - vMousePos;

				m_vSCScale = Vec2(abs(vPosDiff.x), abs(vPosDiff.y));

				// 우상단, 좌하단 좌표 구하기
				// Start와 mousePos 중 작은 x, y의 조합이 우상단
				vLT.x = min(m_vStartPos.x, vMousePos.x);
				vLT.y = min(m_vStartPos.y, vMousePos.y);

				vRB.x = max(m_vStartPos.x, vMousePos.x);
				vRB.y = max(m_vStartPos.y, vMousePos.y);

				m_vSCPos = Vec2(vLT.x + m_vSCScale.x / 2, vLT.y + m_vSCScale.y / 2);
			}
			if (KEY_AWAY(KEY::LBTN))
			{
				m_bDrawingSqare = false;
				m_vEndPos = vMousePos;
				// SceneChanger 생성 후에는 어떤 씬으로 이동하는 SceneChanger인지 결정하는 창이 뜬다.
				// 결정이 되면 결과값을 SceneChanger의 멤버로 채운다 (생성자 이용)
				CScene::CreateSceneChanger(m_vSCPos, m_vSCScale, eScene);
			}
		}
		break;
		default:
			break;
		}
	}
	// Start씬으로 전환
	if (KEY_HOLD(KEY::LSHIFT) && KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::STAGE_01); // 이벤트 만들음
	}
}

void CScene_Tool::render(HDC _dc)
{
	if (m_bGrid)
	{
		PEN_TYPE ePen = PEN_TYPE::BLUE;
		// 객체의 생성자를 이용하는 방식
		SelectGDI p(_dc, ePen);
		SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);

		// 타일이 존재하는 부분에만 그리드 렌더
		UINT tileX = CScene::GetTileX();
		UINT tileY = CScene::GetTileY();

		Vec2 vRenderPos = {};
		Vec2 vPos = {};

		for (UINT i = 0; i < tileY; ++i)
		{
			for (UINT j = 0; j < tileX; ++j)
			{
				vPos.x = j * TILE_SIZE;
				vPos.y = i * TILE_SIZE;

				vRenderPos = CCamera::GetInst()->GetRenderPos(vPos);

				Rectangle(_dc
					, (int)(vRenderPos.x)
					, (int)(vRenderPos.y)
					, vRenderPos.x + TILE_SIZE
					, vRenderPos.y + TILE_SIZE);
			}
		}
	}

	if (m_bDrawingSqare)
	{
		PEN_TYPE ePen = PEN_TYPE::GREEN;
		// 객체의 생성자를 이용하는 방식
		SelectGDI p(_dc, ePen);
		SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);

		Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vSCPos);

		Rectangle(_dc
			, (int)(vRenderPos.x - m_vSCScale.x / 2.f)
			, (int)(vRenderPos.y - m_vSCScale.y / 2.f)
			, (int)(vRenderPos.x + m_vSCScale.x / 2.f)
			, (int)(vRenderPos.y + m_vSCScale.y / 2.f));
	}


	CScene::render(_dc);
}

void CScene_Tool::SaveSceneData()
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
	ofn.lpstrFilter = L"ALL\0*.*\0Scene\0*.scene\0";
	// 필터 인덱스: 골라놓은 것 중 초기 필터를 무엇으로 세팅하는가?
	// 0선택: ALL이라는 뜻...
	ofn.nFilterIndex = 0;
	//창 오픈 시 캡션인데, 수정 가능한 값 줘야해서 일단 패스
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"scene";
	// 저장창의 초기 경로
	ofn.lpstrInitialDir = strTileFolder.c_str();
	// 패스가 존재하고 파일이 존재해야 한다 (비트연산 조합)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// 세팅한 정보 바탕으로 창 열기
	if (GetSaveFileName(&ofn))
	{
		SaveScene(szName);
	}
}

void CScene_Tool::SaveScene(const wstring& _strFilePath)
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

	// 벽 개수 저장
	const vector<CObject*>& vWall = GetGroupObject(GROUP_TYPE::WALL);
	UINT wallSize = vWall.size();

	const vector<CObject*>& vCornerWall = GetGroupObject(GROUP_TYPE::CORNER);
	UINT CornerSize = vCornerWall.size();

	const vector<CObject*>& vTileWall = GetGroupObject(GROUP_TYPE::TILE_WALL);
	UINT TileWallSize = vTileWall.size();

	UINT size = wallSize + CornerSize + TileWallSize;

	// 입력할 데이터의 시작주소 / 입력할 데이터의 크기 / 개수(배열일 경우 배열의크기) / 
	// 시작주소가 void포인터인데, 어떤 데이터든 받을 수 있게 하기 위함임.
	// 일단 받은 다음, 두 번째 인자 크기 단위로 읽는다.
	// 저장할 데이터가 무엇이든 간에 범용적 적용을 위함.
	fwrite(&size, sizeof(UINT), 1, pFile);


	// 모든 Wall을 순회하며 자기 자신을 저장
	for (size_t i = 0; i < wallSize; ++i)
	{
		((CWall*)vWall[i])->Save(pFile);
	}
	for (size_t i = 0; i < CornerSize; ++i)
	{
		((CWall*)vCornerWall[i])->Save(pFile);
	}
	for (size_t i = 0; i < TileWallSize; ++i)
	{
		((CWall*)vTileWall[i])->Save(pFile);
	}


	// 플레이어 저장
	const vector<CObject*>& vecPlayer = GetGroupObject(GROUP_TYPE::PLAYER);
	if (vecPlayer.size() >= 1)
		((CPlayer*)vecPlayer[0])->Save(pFile);

	// 몬스터 저장
	// 몬스터 개수 저장
	const vector<CObject*>& vecMonster = GetGroupObject(GROUP_TYPE::MONSTER);
	UINT monsterSize = vecMonster.size();
	fwrite(&monsterSize, sizeof(UINT), 1, pFile);
	// 개별 몬스터의 정보를 세이브
	for (size_t i = 0; i < monsterSize; ++i)
	{
		((CMonster*)vecMonster[i])->Save(pFile);
	}

	// Scene Changer 저장
	
	// 기타 오브젝트 저장


	// 이 파일에 대한 파일 입출력 닫기
	fclose(pFile);

}

void CScene_Tool::LoadSceneData()
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
	ofn.lpstrFilter = L"ALL\0*.*\0Scene\0*.scene\0";
	// 필터 인덱스: 골라놓은 것 중 초기 필터를 무엇으로 세팅하는가?
	// 0선택: ALL이라는 뜻...
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"scene";
	// 저장창의 초기 경로
	ofn.lpstrInitialDir = strTileFolder.c_str();
	// 패스가 존재하고 파일이 존재해야 한다 (비트연산 조합)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	// 세팅한 정보 바탕으로 오픈 창 열기
	if (GetOpenFileName(&ofn))
	{
		// 상대경로 변환
		wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(szName);
		LoadScene(strRelativePath);
	}
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

		//LoadTile(strRelativePath);
	}
}

void CScene_Tool::SaveWallData()
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
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.wall\0";
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
		SaveWall(szName);
	}
}

void CScene_Tool::SaveWall(const wstring& _strFilePath)
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
	//UINT xCount = GetTileX();
	//UINT yCount = GetTileY();


	// 벽 개수 저장
	const vector<CObject*>& vWall = GetGroupObject(GROUP_TYPE::WALL);
	UINT wallSize = vWall.size();
	
	const vector<CObject*>& vCornerWall = GetGroupObject(GROUP_TYPE::CORNER);
	UINT CornerSize = vCornerWall.size();
	
	const vector<CObject*>& vTileWall = GetGroupObject(GROUP_TYPE::TILE_WALL);
	UINT TileWallSize = vTileWall.size();

	UINT size = wallSize + CornerSize + TileWallSize;

	// 입력할 데이터의 시작주소 / 입력할 데이터의 크기 / 개수(배열일 경우 배열의크기) / 
	// 시작주소가 void포인터인데, 어떤 데이터든 받을 수 있게 하기 위함임.
	// 일단 받은 다음, 두 번째 인자 크기 단위로 읽는다.
	// 저장할 데이터가 무엇이든 간에 범용적 적용을 위함.
	fwrite(&size, sizeof(UINT), 1, pFile);


	// 모든 타일을 개별적으로, 저장할 데이터 저장하게 함
	// 타일 전체를 순회하며 각자 타일에게 저장할거있음 저장해라, save 함수 호출
	// 타일들은 각각 자신을 save 한다
	for (size_t i = 0; i < wallSize; ++i)
	{
		((CWall*)vWall[i])->Save(pFile);
	}
	for (size_t i = 0; i < CornerSize; ++i)
	{
		((CWall*)vCornerWall[i])->Save(pFile);
	}
	for (size_t i = 0; i < TileWallSize; ++i)
	{
		((CWall*)vTileWall[i])->Save(pFile);
	}
	// 이 파일에 대한 파일 입출력 닫기
	fclose(pFile);
}

//어떤 파일을 열지 UI 통해 지정하고, 그것을 Load하기
void CScene_Tool::LoadWallData()
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
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.wall\0";
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

		//LoadWall(strRelativePath);
	}
}



void CScene_Tool::SetSelectedTile(int _idx)
{
	m_iSelectedTileIdx = _idx;
	m_eSelctedObj = SELECT_OPTION::TILE;
}
void CScene_Tool::SetSelectedWall()
{
	m_eSelctedObj = SELECT_OPTION::WALL;
	m_eSelectedWall = WALL_TYPE::WALL;
}
void CScene_Tool::SetSelectedCorner()
{
	m_eSelctedObj = SELECT_OPTION::WALL;
	m_eSelectedWall = WALL_TYPE::CORNER;
}
void CScene_Tool::SetSelectedTileWall(int _idx)
{
	m_iSelectedTileIdx = _idx;
	m_eSelctedObj = SELECT_OPTION::WALL;
	m_eSelectedWall = WALL_TYPE::TILE;
}
void CScene_Tool::SetSelectedPlayer()
{
	m_eSelctedObj = SELECT_OPTION::PLAYER;
}
void CScene_Tool::SetSelectedMosnter()
{
	m_eSelctedObj = SELECT_OPTION::MONSTER;
}
void CScene_Tool::SetSelectedSC()
{
	m_eSelctedObj = SELECT_OPTION::SCENE_CHANGER;
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