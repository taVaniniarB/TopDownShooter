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




CScene::CScene()
	: m_iTileX(0)
	, m_iTileY(0)
	, m_pPlayer(nullptr)
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
	DeleteWall(vMousePos);

	GenerateWall(vMousePos, _eSelectedWallDir1);
	GenerateWall(vMousePos, _eSelectedWallDir2);
}

void CScene::CreateWall(Vec2 vMousePos, WALL_DIR _eSelectedWallDir, WALL_TYPE _eWallType)
{

	switch (_eWallType)
	{
	case WALL_TYPE::WALL:
		DeleteWall(vMousePos);
		GenerateWall(vMousePos, _eSelectedWallDir);
		break;
	case WALL_TYPE::CORNER:
		DeleteCorner(vMousePos);
		GenerateCorner(vMousePos, _eSelectedWallDir);
		break;
	case WALL_TYPE::TILE:
		GenerateTileWall(vMousePos);
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
		vPos = Vec2((iCol * TILE_SIZE), (iRow * TILE_SIZE));
		vScale = Vec2(WALL_THICKNESS, TILE_SIZE);
	}
	break;
	case WALL_DIR::RIGHT:
	{
		vPos = Vec2((iCol * TILE_SIZE) + (TILE_SIZE - WALL_THICKNESS), (iRow * TILE_SIZE));
		vScale = Vec2(WALL_THICKNESS, TILE_SIZE);
	}
	break;
	case WALL_DIR::END:
		break;
	default:
		break;
	}

	pNewWall->SetPos(vPos);
	pNewWall->SetScale(vScale);

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
		vPos = Vec2((iCol * TILE_SIZE), (iRow * TILE_SIZE));
	}
	break;
	case WALL_DIR::BOTTOM:
	{
		vPos = Vec2((iCol * TILE_SIZE), (iRow * TILE_SIZE + (TILE_SIZE - WALL_THICKNESS)));
	}
	break;
	case WALL_DIR::LEFT:
	{
		vPos = Vec2((iCol * TILE_SIZE) + (TILE_SIZE - WALL_THICKNESS), (iRow * TILE_SIZE));
	}
	break;
	case WALL_DIR::RIGHT:
	{
		vPos = Vec2((iCol * TILE_SIZE) + (TILE_SIZE - WALL_THICKNESS), (iRow * TILE_SIZE) + (TILE_SIZE - WALL_THICKNESS));
	}
	break;
	case WALL_DIR::END:
		break;
	default:
		break;
	}

	pNewWall->SetPos(vPos);
	pNewWall->SetScale(vScale);

	AddObject(pNewWall, GROUP_TYPE::CORNER);
}

void CScene::GenerateTileWall(Vec2 vMousePos)
{
	CWall* pNewWall = new CWall;

	Vec2 vPos = {};
	Vec2 vScale = {};

	// ������ǥ ����Ͽ� int
	int iCol = (int)vMousePos.x / TILE_SIZE;
	int iRow = (int)vMousePos.y / TILE_SIZE;

	vPos = Vec2((iCol * TILE_SIZE), (iRow * TILE_SIZE));
	vScale = Vec2(TILE_SIZE, TILE_SIZE);

	pNewWall->SetPos(vPos);
	pNewWall->SetScale(vScale);

	AddObject(pNewWall, GROUP_TYPE::TILE_WALL);
}

void CScene::DeleteWall(Vec2 vMousePos)
{
	// ������ǥ ����Ͽ� int
	int iCol = (int)(vMousePos.x / TILE_SIZE);
	int iRow = (int)(vMousePos.y / TILE_SIZE);

	Vec2 WallPos = Vec2(iCol * TILE_SIZE, iRow * TILE_SIZE);

	HDC dc = CCore::GetInst()->GetMainDC();
	vector<CObject*> vWall = GetGroupObject(GROUP_TYPE::WALL);
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


void CScene::DeleteCorner(Vec2 vMousePos)
{
	// ������ǥ ����Ͽ� int
	int iCol = (int)(vMousePos.x / TILE_SIZE);
	int iRow = (int)(vMousePos.y / TILE_SIZE);

	Vec2 WallPos = Vec2(iCol * TILE_SIZE, iRow * TILE_SIZE);

	HDC dc = CCore::GetInst()->GetMainDC();
	vector<CObject*> vWall = GetGroupObject(GROUP_TYPE::CORNER);
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
void CScene::LoadTile(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// Ŀ�� ������Ʈ
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	assert(pFile);

	// Ÿ�� ���μ��� ���� �ҷ�����
	UINT xCount = 0;
	UINT yCount = 0;

	fread(&xCount, sizeof(UINT), 1, pFile);
	fread(&yCount, sizeof(UINT), 1, pFile);

	// �ҷ��� ������ �°� EmptyTile�� �����α�
	CreateTile(xCount, yCount);

	// ������� ���� ������ �ʿ��� ���� �ҷ����� ��
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Load(pFile);
	}

	// �� ���Ͽ� ���� ���� ����� �ݱ�
	fclose(pFile);

}