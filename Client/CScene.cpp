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

// 벡터: 씬의 멤버이므로, 씬이 소멸할 때 자동으로 사라짐
// 객체: 벡터에는 동적할당 한 객체들의 "주소"를 넣은거라 위와 같이 수동 삭제 해주어야 함
// 객체그자체를 넣었으면 ㄱㅊ았을텐데. 주소를 저장한거니까, 직접가서 조져야만...
// 벡터 안에 열매가 맺히듯 주렁주렁한데 가지만 없앤다고 열매가 사라지지 않음!
CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			// m_arrObj[i] 그룹 벡터의 j 물체 삭제
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

	// 타일 생성
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

	// 음수좌표 고려하여 int
	int iCol = (int)_vMousePos.x / TILE_SIZE;
	int iRow = (int)_vMousePos.y / TILE_SIZE;

	// 타일 없는 곳 클릭 예외처리
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

	// 음수좌표 고려하여 int
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

	// 음수좌표 고려하여 int
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

	// 음수좌표 고려하여 int
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
	// 음수좌표 고려하여 int
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
	// 음수좌표 고려하여 int
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
{ // 작업을 마무리쳐주는 (확정지어주는) 업데이트
	// 충돌체가 오브젝트를 따라가게 한다
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			// Dead 객체라도
			// 층돌체의 경우, 마무리 작업 필요하기 때문에 final update까지는 돌려준다
			m_arrObj[i][j]->finalUpdate();
		}
	}
}

void CScene::render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		// 현재 렌더링 그룹이 타일일 시
		if ((UINT)GROUP_TYPE::TILE == i)
		{
			// 따로 렌더링 함수를 구현한다.
			render_tile(_dc);
			continue;
		}

		// 렌더 전, 객체가 죽은 상태가 아닌지 검사하여 null 참조를 방지한다
		// 렌더에서 이 검사를 하는 이유: finalupdate 등 이미 죽은애와의 충돌하는 건 말이 안 되니까
		// 가장 나중의 작업인 render에서 구현한 것이다.
		// earse 함수 쓰기 위해 operator(i, j...) 변수 쓰지 않고
		// iterator 쓴다

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
				iter = m_arrObj[i].erase(iter); //벡터에서 데이터 삭제 후엔 그 다음 이터 받는 것 기억
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

	// 스크린 좌상단 인덱스
	int iLTCol = (int)vLT.x / TILE_SIZE;

	int iLTRow = (int)vLT.y / TILE_SIZE;

	int iClientWidth = ((int)Res.x / TILE_SIZE) + 1; // 화면 내에 들어오는 타일 개수(가로) / 잘림 방지 위해 + 1
	int iClientHeight = ((int)Res.y / TILE_SIZE) + 1; // 세로 타일 개수

	for (int iCurRow = iLTRow; iCurRow < (iLTRow + iClientHeight); ++iCurRow)
	{
		for (int iCurCol = iLTCol; iCurCol < (iLTCol + iClientWidth); ++iCurCol)
		{
			// 타일이 존재하지 않는 영역을 Lookat 시
			// 그 부분의 인덱스 계산 및 렌더는 패스
			// 예외처리
			// 1. 현재 열은 음수가 될 수 없다
			// 2. 현재 열은 총 타일 가로 개수보다 클 수 없다
			// 3. 현재 행은 음수가 될 수 없다
			// 4. 현재 행은 총 타일 세로 개수보다 클 수 없다
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

// 상대경로 사용하는 이유
// 툴에서는 특정 창 열어서 불러왔지만,
// 씬 enter()에서 절대경로로 파일 불러오는 방식은 좋지 않을 것
void CScene::LoadTile(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// 커널 오브젝트
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	assert(pFile);

	// 타일 가로세로 개수 불러오기
	UINT xCount = 0;
	UINT yCount = 0;

	fread(&xCount, sizeof(UINT), 1, pFile);
	fread(&yCount, sizeof(UINT), 1, pFile);

	// 불러온 개수에 맞게 EmptyTile들 만들어두기
	CreateTile(xCount, yCount);

	// 만들어진 파일 개별로 필요한 정보 불러오게 함
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Load(pFile);
	}

	// 이 파일에 대한 파일 입출력 닫기
	fclose(pFile);

}