#include "pch.h"
#include "CScene.h"
#include "CObject.h"
#include "CTile.h"
#include "CWall.h"
#include "CResMgr.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CCamera.h"
#include "CCore.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CSceneChanger.h"
#include "CUI.h"
#include "CTextUI.h"
#include "SelectGDI.h"



CScene::CScene()
	: m_iTileX(0)
	, m_iTileY(0)
	, m_pPlayer(nullptr)
	, m_bUIClicked(false)
	, m_pStage(nullptr)
	, m_bEnabled(true)
	, m_bGrid(false)
{
}

// ����: ���� ����̹Ƿ�, ���� �Ҹ��� �� �ڵ����� �����
// ��ü: ���Ϳ��� �����Ҵ� �� ��ü���� "�ּ�"�� �����Ŷ� ���� ���� ���� ���� ���־�� ��
// ��ü����ü�� �־����� ���������ٵ�. �ּҸ� �����ѰŴϱ�, �������� �����߸�...
// ���� �ȿ� ���Ű� ������ �ַ��ַ��ѵ� ������ ���شٰ� ���Ű� ������� ����!
CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			// m_arrObj[i] �׷� ������ j ��ü ����
			delete m_arrObj[i][j];
		}
	}

}

void CScene::CreateTile(UINT _iXCount, UINT _iYCount)
{
	DeleteGroup(GROUP_TYPE::TILE);

	m_iTileX = _iXCount;
	m_iTileY = _iYCount;


	CTexture* pTileTex = CResMgr::GetInst()->LoadTexture(L"Tile", L"texture\\tile\\tile.bmp");

	// Ÿ�� ����
	for (UINT i = 0; i < _iYCount; ++i)
	{
		for (UINT j = 0; j < _iXCount; ++j)
		{
			CTile* pTile = new CTile();

			pTile->SetTexture(pTileTex);
			pTile->SetPos(Vec2((float)(TILE_SIZE * j), (float)(i * TILE_SIZE)));
			AddObject(pTile, GROUP_TYPE::TILE);
		}
	}
}

void CScene::ChangeTile(Vec2 _vMousePos, int _idx)
{
	int iTileX = (int)GetTileX();
	int iTileY = (int)GetTileY();

	// ������ǥ ����Ͽ� int
	int iCol = (int)_vMousePos.x / TILE_SIZE;
	int iRow = (int)_vMousePos.y / TILE_SIZE;

	// Ÿ�� ���� �� Ŭ�� ����ó��
	if (_vMousePos.x < 0.f || iCol >= iTileX
		|| _vMousePos.y < 0.f || iRow >= iTileY)
	{
		return;
	}

	int iIdx = iCol + (iRow * m_iTileX);

	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
	CTile* changeTile = dynamic_cast<CTile*>(vecTile[iIdx]);
	changeTile->SetImgIdx(_idx);
}


void CScene::CreateWall(Vec2 vMousePos, WALL_DIR _eSelectedWallDir1, WALL_DIR _eSelectedWallDir2)
{
	DeleteWall(vMousePos, WALL_TYPE::WALL);

	GenerateWall(vMousePos, _eSelectedWallDir1);
	GenerateWall(vMousePos, _eSelectedWallDir2);
}

void CScene::CreateWall(Vec2 vMousePos, WALL_DIR _eSelectedWallDir, WALL_TYPE _eWallType, int _idx)
{

	switch (_eWallType)
	{
	case WALL_TYPE::WALL:
		DeleteWall(vMousePos, _eWallType);
		GenerateWall(vMousePos, _eSelectedWallDir);
		break;
	case WALL_TYPE::CORNER:
		DeleteWall(vMousePos, _eWallType);
		GenerateCorner(vMousePos, _eSelectedWallDir);
		break;
	case WALL_TYPE::TILE:
		ChangeTile(vMousePos, -1);
		DeleteWall(vMousePos, _eWallType);
		GenerateTileWall(vMousePos, _idx);
		break;
	default:
		break;
	}
}


void CScene::GenerateWall(Vec2 vMousePos, WALL_DIR _eSelectedWallDir)
{
	CWall* pNewWall = new CWall;
	
	
	Vec2 vPos = {};
	Vec2 vScale = {};

	// ������ǥ ����Ͽ� int
	int iCol = (int)vMousePos.x / TILE_SIZE;
	int iRow = (int)vMousePos.y / TILE_SIZE;

	switch (_eSelectedWallDir)
	{
	case WALL_DIR::TOP:
	{
		vPos = Vec2((iCol * TILE_SIZE), (iRow * TILE_SIZE));
		vScale = Vec2(TILE_SIZE, WALL_THICKNESS);
	}
	break;
	case WALL_DIR::BOTTOM:
	{
		vPos = Vec2((iCol * TILE_SIZE), (iRow * TILE_SIZE + (TILE_SIZE - WALL_THICKNESS)));
		vScale = Vec2(TILE_SIZE, WALL_THICKNESS);
	}
	break;
	case WALL_DIR::LEFT:
	{
		vPos = Vec2((float)(iCol * TILE_SIZE), (float)(iRow * TILE_SIZE));
		vScale = Vec2(WALL_THICKNESS, TILE_SIZE);
	}
	break;
	case WALL_DIR::RIGHT:
	{
		vPos = Vec2((float)(iCol * TILE_SIZE) + (float)(TILE_SIZE - WALL_THICKNESS), (float)(iRow * TILE_SIZE));
		vScale = Vec2(WALL_THICKNESS, TILE_SIZE);
	}
	break;
	case WALL_DIR::END:
		break;
	default:
		break;
	}

	CTexture* pWallTex = CResMgr::GetInst()->LoadTexture(L"Tile", L"texture\\tile\\tile.bmp");
	pNewWall->SetTexture(pWallTex);
	if (_eSelectedWallDir == WALL_DIR::TOP || _eSelectedWallDir == WALL_DIR::BOTTOM)
		pNewWall->SetImgIdx(9);
	else
		pNewWall->SetImgIdx(8);
	pNewWall->SetPos(vPos);
	pNewWall->SetScale(vScale);
	pNewWall->SetType(WALL_TYPE::WALL);

	AddObject(pNewWall, GROUP_TYPE::WALL);
}

void CScene::GenerateCorner(Vec2 vMousePos, WALL_DIR _eSelectedWallDir)
{
	CWall* pNewWall = new CWall;

	Vec2 vPos = {};
	Vec2 vScale = {};

	// ������ǥ ����Ͽ� int
	int iCol = (int)vMousePos.x / TILE_SIZE;
	int iRow = (int)vMousePos.y / TILE_SIZE;

	vScale = Vec2(WALL_THICKNESS, WALL_THICKNESS);

	switch (_eSelectedWallDir)
	{
	case WALL_DIR::TOP:
	{
		vPos = Vec2((float)(iCol * TILE_SIZE), (float)(iRow * TILE_SIZE));
	}
	break;
	case WALL_DIR::BOTTOM:
	{
		vPos = Vec2((float)(iCol * TILE_SIZE) + (float)(TILE_SIZE - WALL_THICKNESS), (float)(iRow * TILE_SIZE + (TILE_SIZE - WALL_THICKNESS)));
	}
	break;
	case WALL_DIR::LEFT:
	{
		vPos = Vec2((iCol * (float)TILE_SIZE), (float)(iRow * TILE_SIZE) + (float)(TILE_SIZE - WALL_THICKNESS));
	}
	break;
	case WALL_DIR::RIGHT:
	{
		vPos = Vec2((iCol * (float)TILE_SIZE) + (float)(TILE_SIZE - WALL_THICKNESS), (float)((iRow * TILE_SIZE)));
	}
	break;
	case WALL_DIR::END:
		break;
	default:
		break;
	}

	CTexture* pWallTex = CResMgr::GetInst()->LoadTexture(L"Tile", L"texture\\tile\\tile.bmp");
	pNewWall->SetTexture(pWallTex);
	pNewWall->SetImgIdx(10);
	pNewWall->SetPos(vPos);
	pNewWall->SetScale(vScale);
	pNewWall->SetType(WALL_TYPE::CORNER);

	AddObject(pNewWall, GROUP_TYPE::CORNER);
}

void CScene::GenerateTileWall(Vec2 vMousePos, int _idx)
{
	CWall* pNewWall = new CWall;

	Vec2 vPos = {};
	Vec2 vScale = {};

	// ������ǥ ����Ͽ� int
	int iCol = (int)vMousePos.x / TILE_SIZE;
	int iRow = (int)vMousePos.y / TILE_SIZE;

	vPos = Vec2((float)(iCol * TILE_SIZE), (float)(iRow * TILE_SIZE));
	vScale = Vec2(TILE_SIZE, TILE_SIZE);

	CTexture* pWallTex = CResMgr::GetInst()->LoadTexture(L"Tile", L"texture\\tile\\tile.bmp");
	pNewWall->SetTexture(pWallTex);
	pNewWall->SetImgIdx(_idx);
	pNewWall->SetPos(vPos);
	pNewWall->SetScale(vScale);
	pNewWall->SetType(WALL_TYPE::TILE);
	
	AddObject(pNewWall, GROUP_TYPE::TILE_WALL);
}

void CScene::DeleteWall(Vec2 vMousePos, WALL_TYPE eType)
{
	// ������ǥ ����Ͽ� int
	int iCol = (int)(vMousePos.x / TILE_SIZE);
	int iRow = (int)(vMousePos.y / TILE_SIZE);

	Vec2 WallPos = Vec2(iCol * TILE_SIZE, iRow * TILE_SIZE);

	HDC dc = CCore::GetInst()->GetMainDC();
	
	const vector<CObject*>& vWall = GetGroupObject(GROUP_TYPE((int(eType)+2)));

	switch (eType)
	{
	case WALL_TYPE::WALL:
		//vWall = GetGroupObject(GROUP_TYPE::WALL);
		break;
	case WALL_TYPE::CORNER:
	{
		//vWall = GetGroupObject(GROUP_TYPE::CORNER);
		for (int i = 0; i < vWall.size(); ++i)
		{
			Vec2 vCompare = vWall[i]->GetPos();

			for (int j = 0; j < (int)WALL_DIR::END; ++j)
			{
				if (WallPos.x == vCompare.x && WallPos.y == vCompare.y
					|| WallPos.x == vCompare.x && WallPos.y + (TILE_SIZE - WALL_THICKNESS) == vCompare.y
					|| WallPos.x + (TILE_SIZE - WALL_THICKNESS) == vCompare.x && WallPos.y == vCompare.y
					|| WallPos.x + (TILE_SIZE - WALL_THICKNESS) == vCompare.x && WallPos.y + (TILE_SIZE - WALL_THICKNESS) == vCompare.y)
				{
					DeleteObject(vWall[i]);
					break;
				}
			}
		}
		return;
	}
		break;
	case WALL_TYPE::TILE:
		//vWall = GetGroupObject(GROUP_TYPE::TILE_WALL);
		break;
	default:
		break;
	}


	
	for (int i = 0; i < vWall.size(); ++i)
	{
		Vec2 vCompare = vWall[i]->GetPos();

		for (int j = 0; j < (int)WALL_DIR::END; ++j)
		{
			if (WallPos.x == vCompare.x && WallPos.y == vCompare.y // Left, Top
				|| WallPos.x == vCompare.x && WallPos.y + (TILE_SIZE - WALL_THICKNESS) == vCompare.y // Bottom
				|| WallPos.x + (TILE_SIZE - WALL_THICKNESS) == vCompare.x && WallPos.y == vCompare.y) // Right
			{
				DeleteObject(vWall[i]);
				break;
			}
		}
	}
}

void CScene::SpawnPlayer(Vec2 vMousePos)
{
	CObject* pObj = new CPlayer;
	pObj->SetPos(vMousePos);
	pObj->SetName(L"Player");
	pObj->SetScale(Vec2(100.f, 100.f));
	pObj->SetEnabled(false);
	AddObject(pObj, GROUP_TYPE::PLAYER);
}

void CScene::SpawnMonster(Vec2 vMousePos)
{
	CMonster* pMon = CMonFactory::CreateMonster(MON_TYPE::NORMAL, vMousePos);
	CObject* pObj = (CObject*)pMon;
	pObj->SetEnabled(false);
	AddObject(pObj, GROUP_TYPE::MONSTER);
}

void CScene::CreateSceneChanger(Vec2 vPos, Vec2 vScale, SCENE_TYPE _eScene)
{
	CObject* pObj = new CSceneChanger(_eScene);
	pObj->SetPos(vPos);
	pObj->SetScale(vScale);
	AddObject(pObj, GROUP_TYPE::SCENE_CHANGER);
}

void CScene::SetUIVisable(wstring _strUIName, bool _b)
{
	FindUI(_strUIName)->SetVisable(_b);
}

void CScene::SetUIText(wstring _strUIName, wstring _strText)
{
	((CTextUI*)FindUI(_strUIName))->SetText(_strText);
}

void CScene::SetUIText(wstring _strUIName, int _iNum)
{
	((CTextUI*)FindUI(_strUIName))->SetText(_iNum);
}

CUI* CScene::FindUI(std::wstring& _strUIName)
{
	vector<CObject*> UIVec = GetGroupObject(GROUP_TYPE::UI);
	for (size_t i = 0; i < UIVec.size(); ++i)
	{
		if (UIVec[i]->GetName() == _strUIName)
		{
			return ((CUI*)UIVec[i]);
		}
	}
	return nullptr;
}

void CScene::start()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->start();
		}
	}
}

void CScene::update()
{
#ifdef _DEBUG
	// �׸��� On/Off
	if (KEY_TAP(KEY::G))
	{
		SetGrid();
	}
#endif
	if (m_bEnabled)
	{
		for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
		{
			for (size_t j = 0; j < m_arrObj[i].size(); ++j)
			{
				if (!m_arrObj[i][j]->IsDead())
				{
					m_arrObj[i][j]->update();
				}
			}
		}
	}
}

void CScene::finalUpdate()
{ // �۾��� ���������ִ� (Ȯ�������ִ�) ������Ʈ
	// �浹ü�� ������Ʈ�� ���󰡰� �Ѵ�
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			// Dead ��ü��
			// ����ü�� ���, ������ �۾� �ʿ��ϱ� ������ final update������ �����ش�
			m_arrObj[i][j]->finalUpdate();
		}
	}
}

void CScene::render(HDC _dc)
{

	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		// ���� ������ �׷��� Ÿ���� ��
		if ((UINT)GROUP_TYPE::TILE == i)
		{
			// ���� ������ �Լ��� �����Ѵ�.
			render_tile(_dc);
			continue;
		}

		// ���� ��, ��ü�� ���� ���°� �ƴ��� �˻��Ͽ� null ������ �����Ѵ�
		// �������� �� �˻縦 �ϴ� ����: finalupdate �� �̹� �����ֿ��� �浹�ϴ� �� ���� �� �Ǵϱ�
		// ���� ������ �۾��� render���� ������ ���̴�.
		// earse �Լ� ���� ���� operator(i, j...) ���� ���� �ʰ�
		// iterator ����

		vector<CObject*>::iterator iter = m_arrObj[i].begin();

		for (; iter != m_arrObj[i].end();)
		{
			if (!(*iter)->IsDead())
			{
				(*iter)->render(_dc);
				++iter;

			}
			else
			{
				iter = m_arrObj[i].erase(iter); //���Ϳ��� ������ ���� �Ŀ� �� ���� ���� �޴� �� ���
			}
		}
	}

	render_grid(_dc);
}

void CScene::render_grid(HDC _dc)
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
}

void CScene::render_tile(HDC _dc)
{
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	Vec2 CameraLookat = CCamera::GetInst()->GetLookAt();
	Vec2 Res = CCore::GetInst()->GetResolution();
	Vec2 vLT = { (CameraLookat.x - Res.x / 2.f), (CameraLookat.y - Res.y / 2.f) };

	// ��ũ�� �»�� �ε���
	int iLTCol = (int)vLT.x / TILE_SIZE;

	int iLTRow = (int)vLT.y / TILE_SIZE;

	int iClientWidth = ((int)Res.x / TILE_SIZE) + 1; // ȭ�� ���� ������ Ÿ�� ����(����) / �߸� ���� ���� + 1
	int iClientHeight = ((int)Res.y / TILE_SIZE) + 1; // ���� Ÿ�� ����

	for (int iCurRow = iLTRow; iCurRow < (iLTRow + iClientHeight); ++iCurRow)
	{
		for (int iCurCol = iLTCol; iCurCol < (iLTCol + iClientWidth); ++iCurCol)
		{
			// Ÿ���� �������� �ʴ� ������ Lookat ��
			// �� �κ��� �ε��� ��� �� ������ �н�
			// ����ó��
			// 1. ���� ���� ������ �� �� ����
			// 2. ���� ���� �� Ÿ�� ���� �������� Ŭ �� ����
			// 3. ���� ���� ������ �� �� ����
			// 4. ���� ���� �� Ÿ�� ���� �������� Ŭ �� ����
			if ((UINT)iCurCol < 0 || m_iTileX <= (UINT)iCurCol
				|| iCurRow < 0 || m_iTileY <= (UINT)iCurRow)
			{
				continue;
			}

			int iIdx = (m_iTileX * iCurRow) + iCurCol;

			vecTile[iIdx]->render(_dc);
		}
	}
}

int CScene::GetWallmapNum(int x, int y)
{
	return m_WallMap[y][x];
}

void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
	Safe_Delete_Vec<CObject*>(m_arrObj[(UINT)_eTarget]);
}

void CScene::DeleteAll()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		DeleteGroup((GROUP_TYPE)i);
	}
}

// ����� ����ϴ� ����
// �������� Ư�� â ��� �ҷ�������,
// �� enter()���� �����η� ���� �ҷ����� ����� ���� ���� ��
void CScene::LoadTile(const wstring& _strRelativePath, FILE* _pFile)
{
	// Ÿ�� ���μ��� ���� �ҷ�����
	UINT xCount = 0;
	UINT yCount = 0;

	fread(&xCount, sizeof(UINT), 1, _pFile);
	fread(&yCount, sizeof(UINT), 1, _pFile);

	// �ҷ��� ������ �°� EmptyTile�� �����α�
	CreateTile(xCount, yCount);

	// ������� ���� ������ �ʿ��� ���� �ҷ����� ��
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Load(_pFile);
	}

	// map ����
	m_WallMap = vector<vector<int>>((int)yCount, vector<int>(xCount, 0));
}

void CScene::LoadWall(const wstring& _strRelativePath, FILE* _pFile)
{
	UINT size = 0;

	fread(&size, sizeof(UINT), 1, _pFile);

	// ������ �´� Wall ����
	for (UINT i = 0; i < size; ++i)
	{
		CWall* pWall = new CWall;
		pWall->Load(_pFile);

		// �׸��� ���� ���� �ִ��� ���θ� ��Ÿ���� map ä���
		int iCol = (pWall->GetPos() / TILE_SIZE).x;
		int iRow = (pWall->GetPos() / TILE_SIZE).y;

		m_WallMap[iRow][iCol] = 1;
	}

}

void CScene::LoadPlayer(const wstring& _strRelativePath, FILE* _pFile)
{
	CPlayer* pPlayer = new CPlayer;
	pPlayer->Load(_pFile);
}

void CScene::LoadMonster(const wstring& _strRelativePath, FILE* _pFile)
{
	UINT size = 0;

	fread(&size, sizeof(UINT), 1, _pFile);

	// ������ �´� Monster ����
	for (UINT i = 0; i < size; ++i)
	{
		MON_TYPE type;
		fread(&type, sizeof(MON_TYPE), 1, _pFile);
		
		Vec2 vPos;
		fread(&vPos, sizeof(vPos), 1, _pFile);

		CMonster* pMon = CMonFactory::CreateMonster(type, vPos);

		pMon->Load(_pFile);
	}
}

void CScene::LoadSceneChanger(const wstring& _strRelativePath, FILE* _pFile)
{
	SCENE_TYPE eScene;
	fread(&eScene, sizeof(SCENE_TYPE), 1, _pFile);
	CSceneChanger* pSceneChanger = new CSceneChanger(eScene);
	pSceneChanger->Load(_pFile);
}


void CScene::LoadScene(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// Ŀ�� ������Ʈ
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	assert(pFile);

	LoadTile(strFilePath, pFile);
	LoadWall(strFilePath, pFile);
	LoadPlayer(strFilePath, pFile);
	LoadMonster(strFilePath, pFile);
	LoadSceneChanger(strFilePath, pFile);
	// LoadObject(strFilePath, pFile);


	fclose(pFile);
}
