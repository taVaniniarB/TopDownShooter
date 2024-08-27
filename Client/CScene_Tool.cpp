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


// �Լ������� ���� �����ؾ� �ϴµ� �����Լ��� �Ʒ��� �־ ���漱��
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
	// Ÿ�� ����
	CreateTile(5, 5);
	
	// UI ����
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	CUI* pPanelUI = new CPanelUI;

	pPanelUI->SetName(L"ParentUI");
	pPanelUI->SetScale(Vec2(300.f, 200.f));
	pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x, 0.f));

	CBtnUI* pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"ChildUI");
	pBtnUI->SetScale(Vec2(100.f, 40.f));
	// �θ�κ��� ����� ��ġ
	pBtnUI->SetPos(Vec2(0.f, 0.f));

	// ��ư Ŭ���� ���̺��ϵ��� ����
	// ����Լ� �������� ��� �Լ��� �տ� & �ٿ���߸� �ּҷ� �ν�
	// BtnUI���� �θ� Ŭ������ �Լ��� �޵��� �������Ƿ� �θ� ������Ʈ�� ĳ���� �ʿ�
	((CBtnUI*)pBtnUI)->SetClickedCallBack(this, (SCENE_MEMFUNC)&CScene_Tool::SaveTileData);
	
	pPanelUI->AddChild(pBtnUI);

	// pUI �ϳ��� ���� �־�θ�, ���������� �ڽ��� ȣ��
	AddObject(pPanelUI, GROUP_TYPE::UI);


    // ���� - Ÿ�� UI
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
		pTileSelectUI->SetName(name); //�� wchar ���� ��ġ�� �Լ��� Ÿ�Ͽ� �̸��� �ο�

		int iXScale = (int)(pTilePanelUI->GetScale().x / iMaxRow);
		int iTileUIScale = 80;
		
		pTileSelectUI->SetScale(Vec2((float)iTileUIScale, (float)iTileUIScale));
		iRow = i / iMaxRow;
		iCol = (int)(i % iMaxRow);
		pTileSelectUI->SetPos(Vec2((float)iCol * iXScale, (float)iRow * iXScale));

		((CBtnUI*)pTileSelectUI)->SetClickedCallBack(this, (SCENE_MEMFUNC_INT)&CScene_Tool::SetSelectedTile, i);
		pTilePanelUI->AddChild(pTileSelectUI);
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
	DeleteAll();
}




void CScene_Tool::update()
{
	CScene::update();

	// ���콺 ��ǥ �޾Ƽ� Ÿ�� �ε����� ��ȯ
	SetTileIdx();

	// GŰ�� ������ ������ ����ϰ� �ִ� UI�� ���� ��Ŀ��
	if (KEY_TAP(KEY::G))
	{
		CUIMgr::GetInst()->SetFocusedUI(m_pUI);
	}

	// ��Ʈ�� + S ������ Ÿ���� ����
	if (KEY_TAP(KEY::S) && KEY_HOLD(KEY::CTRL))
	{
		SaveTileData();
	}

	// F ������ ���� �ε�
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

		// Ŭ���� Ÿ���� �ε����� ����
		ChangeTile(vMousePos, m_pSelectedTileIdx);
	}
}


void CScene_Tool::SetTileIdx()
{
	if (KEY_TAP(KEY::LBTN))
	{
		// ���콺 ��ġ�� ������ǥ�� ��ȯ
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);

		int iTileX = (int)GetTileX();
		int iTileY = (int)GetTileY();

		// ������ǥ ����Ͽ� int
		int iCol = (int)vMousePos.x / TILE_SIZE;
		int iRow = (int)vMousePos.y / TILE_SIZE;

		// Ÿ�� ���� �� Ŭ�� ����ó��
		if (vMousePos.x < 0.f || iCol >= iTileX
			|| vMousePos.y < 0.f || iRow >= iTileY)
		{
			return;
		}


		UINT iIdx = iRow * iTileX + iCol;

		const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
		// CObject �������̱� ������, CTile Ÿ������ ĳ����
		// �ڿ������� ������ �����Ͽ� ��ȣ ���ֱ�
		((CTile*)vecTile[iIdx])->AddImgIdx();
	}
	// ���콺 ��ǥ�� ����Ų Ÿ���� � Ÿ������ �˾Ƴ���
	// ���콺 ��ǥ.x, y�� ���� Ÿ�� ������� ������ �� ��° ��, ������ �� �� �ִ�
	// �̷��� ���� ��, ����, Ÿ�� ���� �� �Է��� �ִ� ��, �� ���� ������ ������Ʈ�� Ÿ�� �׷� ���͸� ��ȸ
	// ���� �ε����� ���� �ݺ��� �� ���� �ٷ� ���� ����
	
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
		
		LoadTile(strRelativePath);
	}
}

void CScene_Tool::SetSelectedTile(int _idx)
{
	m_pSelectedTileIdx = _idx;
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