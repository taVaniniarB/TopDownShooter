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

// �Լ������� ���� �����ؾ� �ϴµ� �����Լ��� �Ʒ��� �־ ���漱��
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

	// �������� ����� �޴� �߰�
	CCore::GetInst()->DockMenu();

	// Ÿ�� ����
	CreateTile(50, 30);

	// UI ����
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	//CUI* pPanelUI = new CPanelUI;
	//pPanelUI->SetName(L"ParentUI");
	//pPanelUI->SetScale(Vec2(300.f, 200.f));
	//pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x, 0.f));
	//CBtnUI* pBtnUI = new CBtnUI;
	//pBtnUI->SetName(L"ChildUI");
	//pBtnUI->SetScale(Vec2(100.f, 40.f));
	//// �θ�κ��� ����� ��ġ
	//pBtnUI->SetPos(Vec2(0.f, 0.f));
	//// ��ư Ŭ���� ���̺��ϵ��� ����
	//// ����Լ� �������� ��� �Լ��� �տ� & �ٿ���߸� �ּҷ� �ν�
	//// BtnUI���� �θ� Ŭ������ �Լ��� �޵��� �������Ƿ� �θ� ������Ʈ�� ĳ���� �ʿ�
	//((CBtnUI*)pBtnUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveTileData);
	//pPanelUI->AddChild(pBtnUI);
	//// pUI �ϳ��� ���� �־�θ�, ���������� �ڽ��� ȣ��
	//AddObject(pPanelUI, GROUP_TYPE::UI);


	// Ÿ�� UI
	CUI* pTilePanelUI = new CPanelUI;
	pTilePanelUI->SetName(L"TileParentUI");
	pTilePanelUI->SetScale(Vec2(200.f, 400.f));
	pTilePanelUI->SetPos(Vec2(vResolution.x - pTilePanelUI->GetScale().x, 100.f));
	AddObject(pTilePanelUI, GROUP_TYPE::UI);

	// Ÿ�� �ڽ� UI
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

		// Ÿ�� UI�� �ؽ�ó
		CTexture* pTex = CResMgr::GetInst()->FindTexture(L"Tile");
		pTileSelectUI->SetTexture(pTex);
		pTileSelectUI->SetIdx(i);
	}
	// Ÿ�Ϻ� ��ư
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

		// Ÿ�� UI�� �ؽ�ó
		CTexture* pTex = CResMgr::GetInst()->FindTexture(L"Tile");
		pTileSelectUI->SetTexture(pTex);
		pTileSelectUI->SetIdx(i);
	}
	// �� ��ư
	{
		CTileBtnUI* pTileSelectUI = new CTileBtnUI;
		pTileSelectUI->SetScale(Vec2(50.f, 50.f));
		pTileSelectUI->SetPos(Vec2(0.f, 170.f));
		pTileSelectUI->SetName(L"Wall");
		((CTileBtnUI*)pTileSelectUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SetSelectedWall);
		pTilePanelUI->AddChild(pTileSelectUI);
	}
	// �𼭸� ��ư
	{
		CTileBtnUI* pTileSelectUI = new CTileBtnUI;
		pTileSelectUI->SetScale(Vec2(50.f, 50.f));
		pTileSelectUI->SetPos(Vec2(50.f, 170.f));
		pTileSelectUI->SetName(L"Corner");
		((CTileBtnUI*)pTileSelectUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SetSelectedCorner);
		pTilePanelUI->AddChild(pTileSelectUI);
	}

	// �÷��̾� ��ư
	{
		CBtnUI* pPlayerSelectUI = new CBtnUI;
		pPlayerSelectUI->SetScale(Vec2(50.f, 50.f));
		pPlayerSelectUI->SetPos(Vec2(0.f, 220.f));
		pPlayerSelectUI->SetName(L"Player");
		((CBtnUI*)pPlayerSelectUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SetSelectedPlayer);
		pTilePanelUI->AddChild(pPlayerSelectUI);
	}

	// ���� ��ư
	{
		CBtnUI* pMonSelectUI = new CBtnUI;
		pMonSelectUI->SetScale(Vec2(50.f, 50.f));
		pMonSelectUI->SetPos(Vec2(50.f, 220.f));
		pMonSelectUI->SetName(L"Monster");
		((CBtnUI*)pMonSelectUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SetSelectedMosnter);
		pTilePanelUI->AddChild(pMonSelectUI);
	}

	// SceneChanger ��ư
	{
		CBtnUI* pSCSelectUI = new CBtnUI;
		pSCSelectUI->SetScale(Vec2(50.f, 50.f));
		pSCSelectUI->SetPos(Vec2(50.f, 280.f));
		pSCSelectUI->SetName(L"Monster");
		((CBtnUI*)pSCSelectUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SetSelectedSC);
		pTilePanelUI->AddChild(pSCSelectUI);
	}


	// ���纻 UI
	/* ���纻 UI
	CUI* pClonePanel = pPanelUI->Clone();
	pClonePanel->SetPos(pClonePanel->GetPos() + Vec2(-100.f, 100.f));

	// ������ UI�� ��ư�� �ݹ��Լ� ���
	((CBtnUI*)pClonePanel->GetChildUI()[0])->SetClickedCallBack(&ChangeScene, 0, 0);

	AddObject(pClonePanel, GROUP_TYPE::UI);

	m_pUI = pClonePanel;

	*/

	// ī�޶� LookAt ����
	// 	   //���� UI �������� �ػ� �޾ƿ��� �κ� �־, �ϴ� �ּ� ó��
	//Vec2 vResolution = CCore::GetInst()->GetResolution();
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

}

void CScene_Tool::Exit()
{
	CCamera::GetInst()->FadeOut(1.f);
	// ȭ���� ��� ������ҵ�...?
	CCore::GetInst()->DivideMenu();
	DeleteAll();
}




void CScene_Tool::update()
{
	CScene::update();

	// GŰ�� ������ ������ ����ϰ� �ִ� UI�� ���� ��Ŀ��
	/*
	if (KEY_TAP(KEY::G))
	{
		CUIMgr::GetInst()->SetFocusedUI(m_pUI);
	}
	*/

	// �׸��� On/Off
	if (KEY_TAP(KEY::G))
	{
		m_bGrid = !m_bGrid;
	}

	// ��Ʈ�� + S ������ Ÿ���� ����
	if (KEY_TAP(KEY::S) && KEY_HOLD(KEY::CTRL))
	{
		SaveSceneData();
	}

	// F ������ ���� �ε�
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
			// shift + Ŭ��: ����
			if (KEY_HOLD(KEY::LSHIFT) && KEY_TAP(KEY::LBTN))
			{
				CScene::DeleteWall(vMousePos, m_eSelectedWall);
				break;
			}
			// Ŭ��: wsad + Ŭ��: �ش� ��ġ�� �� ����
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
					// ������ player�� ����
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
				// shift + Ŭ��: ����
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
					// ������ scene changer ����
					DeleteObject(sceneChangerVec[0]);
				}

				// ������ ����
				m_vStartPos = vMousePos;
			}
			if (KEY_HOLD(KEY::LBTN))
			{
				m_bDrawingSqare = true;

				// ���콺 �巡�׷� SceneChanger�� Position�� Scale�� ������.
				
				Vec2 vLT, vRB;
				
				// ���� ���콺 ��ġ -> Start ��ġ ���ϴ� ����
				Vec2 vPosDiff = m_vStartPos - vMousePos;

				m_vSCScale = Vec2(abs(vPosDiff.x), abs(vPosDiff.y));

				// ����, ���ϴ� ��ǥ ���ϱ�
				// Start�� mousePos �� ���� x, y�� ������ ����
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
				// SceneChanger ���� �Ŀ��� � ������ �̵��ϴ� SceneChanger���� �����ϴ� â�� ���.
				// ������ �Ǹ� ������� SceneChanger�� ����� ä��� (������ �̿�)
				CScene::CreateSceneChanger(m_vSCPos, m_vSCScale, eScene);
			}
		}
		break;
		default:
			break;
		}
	}
	// Start������ ��ȯ
	if (KEY_HOLD(KEY::LSHIFT) && KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::STAGE_01); // �̺�Ʈ ������
	}
}

void CScene_Tool::render(HDC _dc)
{
	if (m_bGrid)
	{
		PEN_TYPE ePen = PEN_TYPE::BLUE;
		// ��ü�� �����ڸ� �̿��ϴ� ���
		SelectGDI p(_dc, ePen);
		SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);

		// Ÿ���� �����ϴ� �κп��� �׸��� ����
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
		// ��ü�� �����ڸ� �̿��ϴ� ���
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
	// â�� ����� ��� ���� �������� ���ϰ�
	// �� ��� �޾ƿͼ� SaveTile �Լ��� ȣ������.

	OPENFILENAME ofn = {};

	// ����� ������ �����θ� ���� ����
	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();

	// �ϼ��� ��ΰ� ä���� �ּ�(�迭�� �����ּ�)�� ����Ų��
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName); // ���� ũ�� (����Ʈ)
	// lpstrFilter: Ȯ���� ���� ��Ģ
	ofn.lpstrFilter = L"ALL\0*.*\0Scene\0*.scene\0";
	// ���� �ε���: ������ �� �� �ʱ� ���͸� �������� �����ϴ°�?
	// 0����: ALL�̶�� ��...
	ofn.nFilterIndex = 0;
	//â ���� �� ĸ���ε�, ���� ������ �� ����ؼ� �ϴ� �н�
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"scene";
	// ����â�� �ʱ� ���
	ofn.lpstrInitialDir = strTileFolder.c_str();
	// �н��� �����ϰ� ������ �����ؾ� �Ѵ� (��Ʈ���� ����)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// ������ ���� �������� â ����
	if (GetSaveFileName(&ofn))
	{
		SaveScene(szName);
	}
}

void CScene_Tool::SaveScene(const wstring& _strFilePath)
{
	// Ŀ�� ������Ʈ
	FILE* pFile = nullptr;

	// C++���� �⺻���� �����ϴ� ����� �Լ�
	// ���� �����͸� ���ڷ� �ѱ����ν�, ���� �Ŀ� pFile�� ���� ����Ű�� ��
	// ������������ �ּ� / ������(wchar*) / ���(w: ����)
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");

	// ���� ����(fopen) ����. �����ߴٸ� � �ּҰ� ä���� �����״�...
	assert(pFile);

	// ������ ����
	// ������ ��: Ÿ�� ���μ��� ����
	UINT xCount = GetTileX();
	UINT yCount = GetTileY();

	// �Է��� �������� �����ּ� / �Է��� �������� ũ�� / ����(�迭�� ��� �迭��ũ��) / 
	// �����ּҰ� void�������ε�, � �����͵� ���� �� �ְ� �ϱ� ������.
	// �ϴ� ���� ����, �� ��° ���� ũ�� ������ �д´�.
	// ������ �����Ͱ� �����̵� ���� ������ ������ ����.
	fwrite(&xCount, sizeof(UINT), 1, pFile);
	fwrite(&yCount, sizeof(UINT), 1, pFile);


	// Ÿ�� ������ ������ ���� �ڱ�(Ÿ��) �ʿ� ���� (���̺갡 �ƴ� Ÿ���� ����)
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	// ��� Ÿ���� ����������, ������ ������ �����ϰ� ��
	// Ÿ�� ��ü�� ��ȸ�ϸ� ���� Ÿ�Ͽ��� �����Ұ����� �����ض�, save �Լ� ȣ��
	// Ÿ�ϵ��� ���� �ڽ��� save �Ѵ�
	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Save(pFile);
	}

	// �� ���� ����
	const vector<CObject*>& vWall = GetGroupObject(GROUP_TYPE::WALL);
	UINT wallSize = vWall.size();

	const vector<CObject*>& vCornerWall = GetGroupObject(GROUP_TYPE::CORNER);
	UINT CornerSize = vCornerWall.size();

	const vector<CObject*>& vTileWall = GetGroupObject(GROUP_TYPE::TILE_WALL);
	UINT TileWallSize = vTileWall.size();

	UINT size = wallSize + CornerSize + TileWallSize;

	// �Է��� �������� �����ּ� / �Է��� �������� ũ�� / ����(�迭�� ��� �迭��ũ��) / 
	// �����ּҰ� void�������ε�, � �����͵� ���� �� �ְ� �ϱ� ������.
	// �ϴ� ���� ����, �� ��° ���� ũ�� ������ �д´�.
	// ������ �����Ͱ� �����̵� ���� ������ ������ ����.
	fwrite(&size, sizeof(UINT), 1, pFile);


	// ��� Wall�� ��ȸ�ϸ� �ڱ� �ڽ��� ����
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


	// �÷��̾� ����
	const vector<CObject*>& vecPlayer = GetGroupObject(GROUP_TYPE::PLAYER);
	if (vecPlayer.size() >= 1)
		((CPlayer*)vecPlayer[0])->Save(pFile);

	// ���� ����
	// ���� ���� ����
	const vector<CObject*>& vecMonster = GetGroupObject(GROUP_TYPE::MONSTER);
	UINT monsterSize = vecMonster.size();
	fwrite(&monsterSize, sizeof(UINT), 1, pFile);
	// ���� ������ ������ ���̺�
	for (size_t i = 0; i < monsterSize; ++i)
	{
		((CMonster*)vecMonster[i])->Save(pFile);
	}

	// Scene Changer ����
	
	// ��Ÿ ������Ʈ ����


	// �� ���Ͽ� ���� ���� ����� �ݱ�
	fclose(pFile);

}

void CScene_Tool::LoadSceneData()
{
	// â ��� ��θ� �˾Ƴ��� load tile �ؾ��Ѵ�.

	OPENFILENAME ofn = {};

	// ������ ������ �����θ� ���� ����
	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();

	// ������ ������ ��ΰ� ä���� �ּ�(�迭�� �����ּ�)�� ����Ų��
	ofn.lpstrFile = szName;

	ofn.nMaxFile = sizeof(szName); // ���� ũ�� (����Ʈ)
	// lpstrFilter: Ȯ���� ���� ��Ģ....?
	// ��� ����: L"ALL\0*.*"
	// ���� â ������ �� �ʴ��� Ȯ���� ���ϵ鸸 ��Ͽ� �ߴ°�!!!!
	ofn.lpstrFilter = L"ALL\0*.*\0Scene\0*.scene\0";
	// ���� �ε���: ������ �� �� �ʱ� ���͸� �������� �����ϴ°�?
	// 0����: ALL�̶�� ��...
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"scene";
	// ����â�� �ʱ� ���
	ofn.lpstrInitialDir = strTileFolder.c_str();
	// �н��� �����ϰ� ������ �����ؾ� �Ѵ� (��Ʈ���� ����)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	// ������ ���� �������� ���� â ����
	if (GetOpenFileName(&ofn))
	{
		// ����� ��ȯ
		wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(szName);
		LoadScene(strRelativePath);
	}
}

void CScene_Tool::SaveTileData()
{
	// â�� ����� ��� ���� �������� ���ϰ�
	// �� ��� �޾ƿͼ� SaveTile �Լ��� ȣ������.

	OPENFILENAME ofn = {};

	// ����� ������ �����θ� ���� ����
	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();

	// �ϼ��� ��ΰ� ä���� �ּ�(�迭�� �����ּ�)�� ����Ų��
	ofn.lpstrFile = szName;

	ofn.nMaxFile = sizeof(szName); // ���� ũ�� (����Ʈ)
	// lpstrFilter: Ȯ���� ���� ��Ģ
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	// ���� �ε���: ������ �� �� �ʱ� ���͸� �������� �����ϴ°�?
	// 0����: ALL�̶�� ��...
	ofn.nFilterIndex = 0;
	//â ���� �� ĸ���ε�, ���� ������ �� ����ؼ� �ϴ� �н�
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";
	// ����â�� �ʱ� ���
	ofn.lpstrInitialDir = strTileFolder.c_str();
	// �н��� �����ϰ� ������ �����ؾ� �Ѵ� (��Ʈ���� ����)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	// ������ ���� �������� â ����
	if (GetSaveFileName(&ofn))
	{
		SaveTile(szName);
	}
}

void CScene_Tool::SaveTile(const wstring& _strFilePath)
{
	// Ŀ�� ������Ʈ
	FILE* pFile = nullptr;

	// C++���� �⺻���� �����ϴ� ����� �Լ�
	// ���� �����͸� ���ڷ� �ѱ����ν�, ���� �Ŀ� pFile�� ���� ����Ű�� ��
	// ������������ �ּ� / ������(wchar*) / ���(w: ����)
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");

	// ���� ����(fopen) ����. �����ߴٸ� � �ּҰ� ä���� �����״�...
	assert(pFile);

	// ������ ����
	// ������ ��: Ÿ�� ���μ��� ����
	UINT xCount = GetTileX();
	UINT yCount = GetTileY();

	// �Է��� �������� �����ּ� / �Է��� �������� ũ�� / ����(�迭�� ��� �迭��ũ��) / 
	// �����ּҰ� void�������ε�, � �����͵� ���� �� �ְ� �ϱ� ������.
	// �ϴ� ���� ����, �� ��° ���� ũ�� ������ �д´�.
	// ������ �����Ͱ� �����̵� ���� ������ ������ ����.
	fwrite(&xCount, sizeof(UINT), 1, pFile);
	fwrite(&yCount, sizeof(UINT), 1, pFile);


	// Ÿ�� ������ ������ ���� �ڱ�(Ÿ��) �ʿ� ���� (���̺갡 �ƴ� Ÿ���� ����)
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);


	// ��� Ÿ���� ����������, ������ ������ �����ϰ� ��
	// Ÿ�� ��ü�� ��ȸ�ϸ� ���� Ÿ�Ͽ��� �����Ұ����� �����ض�, save �Լ� ȣ��
	// Ÿ�ϵ��� ���� �ڽ��� save �Ѵ�
	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Save(pFile);
	}


	// �� ���Ͽ� ���� ���� ����� �ݱ�
	fclose(pFile);

}
//� ������ ���� UI ���� �����ϰ�, �װ��� Load�ϱ�
void CScene_Tool::LoadTileData()
{
	// â ��� ��θ� �˾Ƴ��� load tile �ؾ��Ѵ�.

	OPENFILENAME ofn = {};

	// ������ ������ �����θ� ���� ����
	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();

	// ������ ������ ��ΰ� ä���� �ּ�(�迭�� �����ּ�)�� ����Ų��
	ofn.lpstrFile = szName;

	ofn.nMaxFile = sizeof(szName); // ���� ũ�� (����Ʈ)
	// lpstrFilter: Ȯ���� ���� ��Ģ....?
	// ��� ����: L"ALL\0*.*"
	// ���� â ������ �� �ʴ��� Ȯ���� ���ϵ鸸 ��Ͽ� �ߴ°�!!!!
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	// ���� �ε���: ������ �� �� �ʱ� ���͸� �������� �����ϴ°�?
	// 0����: ALL�̶�� ��...
	ofn.nFilterIndex = 0;
	//â ���� �� ĸ���ε�, ���� ������ �� ����ؼ� �ϴ� �н�
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";
	// ����â�� �ʱ� ���
	ofn.lpstrInitialDir = strTileFolder.c_str();
	// �н��� �����ϰ� ������ �����ؾ� �Ѵ� (��Ʈ���� ����)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	// ������ ���� �������� ���� â ����
	if (GetOpenFileName(&ofn))
	{
		// ����� ��ȯ
		wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(szName);

		//LoadTile(strRelativePath);
	}
}

void CScene_Tool::SaveWallData()
{
	// â�� ����� ��� ���� �������� ���ϰ�
	// �� ��� �޾ƿͼ� SaveTile �Լ��� ȣ������.

	OPENFILENAME ofn = {};

	// ����� ������ �����θ� ���� ����
	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();

	// �ϼ��� ��ΰ� ä���� �ּ�(�迭�� �����ּ�)�� ����Ų��
	ofn.lpstrFile = szName;

	ofn.nMaxFile = sizeof(szName); // ���� ũ�� (����Ʈ)
	// lpstrFilter: Ȯ���� ���� ��Ģ
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.wall\0";
	// ���� �ε���: ������ �� �� �ʱ� ���͸� �������� �����ϴ°�?
	// 0����: ALL�̶�� ��...
	ofn.nFilterIndex = 0;
	//â ���� �� ĸ���ε�, ���� ������ �� ����ؼ� �ϴ� �н�
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";
	// ����â�� �ʱ� ���
	ofn.lpstrInitialDir = strTileFolder.c_str();
	// �н��� �����ϰ� ������ �����ؾ� �Ѵ� (��Ʈ���� ����)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	// ������ ���� �������� â ����
	if (GetSaveFileName(&ofn))
	{
		SaveWall(szName);
	}
}

void CScene_Tool::SaveWall(const wstring& _strFilePath)
{
	// Ŀ�� ������Ʈ
	FILE* pFile = nullptr;

	// C++���� �⺻���� �����ϴ� ����� �Լ�
	// ���� �����͸� ���ڷ� �ѱ����ν�, ���� �Ŀ� pFile�� ���� ����Ű�� ��
	// ������������ �ּ� / ������(wchar*) / ���(w: ����)
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");

	// ���� ����(fopen) ����. �����ߴٸ� � �ּҰ� ä���� �����״�...
	assert(pFile);

	// ������ ����
	// ������ ��: Ÿ�� ���μ��� ����
	//UINT xCount = GetTileX();
	//UINT yCount = GetTileY();


	// �� ���� ����
	const vector<CObject*>& vWall = GetGroupObject(GROUP_TYPE::WALL);
	UINT wallSize = vWall.size();
	
	const vector<CObject*>& vCornerWall = GetGroupObject(GROUP_TYPE::CORNER);
	UINT CornerSize = vCornerWall.size();
	
	const vector<CObject*>& vTileWall = GetGroupObject(GROUP_TYPE::TILE_WALL);
	UINT TileWallSize = vTileWall.size();

	UINT size = wallSize + CornerSize + TileWallSize;

	// �Է��� �������� �����ּ� / �Է��� �������� ũ�� / ����(�迭�� ��� �迭��ũ��) / 
	// �����ּҰ� void�������ε�, � �����͵� ���� �� �ְ� �ϱ� ������.
	// �ϴ� ���� ����, �� ��° ���� ũ�� ������ �д´�.
	// ������ �����Ͱ� �����̵� ���� ������ ������ ����.
	fwrite(&size, sizeof(UINT), 1, pFile);


	// ��� Ÿ���� ����������, ������ ������ �����ϰ� ��
	// Ÿ�� ��ü�� ��ȸ�ϸ� ���� Ÿ�Ͽ��� �����Ұ����� �����ض�, save �Լ� ȣ��
	// Ÿ�ϵ��� ���� �ڽ��� save �Ѵ�
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
	// �� ���Ͽ� ���� ���� ����� �ݱ�
	fclose(pFile);
}

//� ������ ���� UI ���� �����ϰ�, �װ��� Load�ϱ�
void CScene_Tool::LoadWallData()
{
	// â ��� ��θ� �˾Ƴ��� load tile �ؾ��Ѵ�.

	OPENFILENAME ofn = {};

	// ������ ������ �����θ� ���� ����
	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();

	// ������ ������ ��ΰ� ä���� �ּ�(�迭�� �����ּ�)�� ����Ų��
	ofn.lpstrFile = szName;

	ofn.nMaxFile = sizeof(szName); // ���� ũ�� (����Ʈ)
	// lpstrFilter: Ȯ���� ���� ��Ģ....?
	// ��� ����: L"ALL\0*.*"
	// ���� â ������ �� �ʴ��� Ȯ���� ���ϵ鸸 ��Ͽ� �ߴ°�!!!!
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.wall\0";
	// ���� �ε���: ������ �� �� �ʱ� ���͸� �������� �����ϴ°�?
	// 0����: ALL�̶�� ��...
	ofn.nFilterIndex = 0;
	//â ���� �� ĸ���ε�, ���� ������ �� ����ؼ� �ϴ� �н�
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";
	// ����â�� �ʱ� ���
	ofn.lpstrInitialDir = strTileFolder.c_str();
	// �н��� �����ϰ� ������ �����ؾ� �Ѵ� (��Ʈ���� ����)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	// ������ ���� �������� ���� â ����
	if (GetOpenFileName(&ofn))
	{
		// ����� ��ȯ
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

// ���� �Լ�
void ChangeScene(DWORD_PTR, DWORD_PTR)
{
	ChangeScene(SCENE_TYPE::START);
}




// Tool Scene�� ��� X, ���� ���ϰ� ��ġ�� �̷��� �صа���
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
			// dialog ���� ������ �������� ���ִ� �Լ�
			// �ڵ�, ID(ID ������ ���ҽ� ��� �������ֱ�), �ʿ����, ����ó��)
			UINT iXCount = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
			UINT iYCount = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);

			// ����� ����Լ��� �ƴϹǷ� SceneMgr�������� scene �޾ƿ;� ��
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			// ������ Tool Scene������ �� UI ����̹Ƿ�, �޴��ٴ� ToolScene������ Ȱ��ȭ�Ѵ�
			// �׸��� �ٿ�ĳ������ ���� �� ������ üũ�����ν� �ڵ� �󿡼��� �޴��ٴ� �� ������ ����� ������� ��������� ǥ���Ѵ�

			// ToolScene Ȯ��
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