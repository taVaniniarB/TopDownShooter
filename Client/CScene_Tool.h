 #pragma once
#include "CScene.h"


enum class SELECT_OPTION
{
	TILE,
	MONSTER,
	WALL,
	PLAYER,
	SCENE_CHANGER,
	WEAPON,

	NONE,
};


class CUI;
class CTile;

class CScene_Tool:
	public CScene
{
private:
	// 강제 포커싱할 UI 저장
	CUI* m_pUI;

	// 현재 선택된 오브젝트 옵션
	SELECT_OPTION m_eSelctedObj;

	// 선택 중인 벽 옵션
	WALL_TYPE m_eSelectedWall;

	// 현재 선택된 타일 인덱스
	int m_iSelectedTileIdx;
	// 현재 선택된 벽 인덱스
	WALL_DIR m_eSelectedWallDir;
	WALL_DIR m_eSelectedWallDir2;

	// 드래그 기능 구현 위한 시작점과 끝점
	Vec2 m_vStartPos;
	Vec2 m_vEndPos;
	Vec2 m_vSCScale;
	Vec2 m_vSCPos;

	// 그리드 표시
	//bool m_bGrid;
	bool m_bDrawingSquare;

public:
	virtual void Enter();
	virtual void Exit();
	virtual void update();
	void Place_Wall(const Vec2& vMousePos);
	void Place_Player(const Vec2& vMousePos);
	virtual void render(HDC _dc);

public:
	void SaveSceneData();
	void SaveScene(const wstring& _strFilePath);
	void LoadSceneData();

	Vec2 GetSCPos() { return m_vSCPos; }
	Vec2 GetSCScale() { return m_vSCScale; }

	// 타일
public:
	// 마우스 클릭 시 타일 인덱스 1 증가
	//void SetTileIdx();
	

	void SaveTileData();
	// 타일을 파일로 저장 (절대경로 사용)
	void SaveTile(const wstring& _strFilePath);
	void LoadTileData();

	void SetSelectedTile(int _idx);
	void SetSelectedWall();
	void SetSelectedCorner();
	void SetSelectedTileWall(int _idx);
	void SetSelectedPlayer();
	void SetSelectedMosnter();
	void SetSelectedSC();
	void SetSelectedWeapon();

	// 벽
public:
	void SaveWallData();
	void SaveWall(const wstring& _strFilePath);
	void LoadWallData();

public:
	void SetSelectedObj(SELECT_OPTION _eOption) { m_eSelctedObj = _eOption; }
	SELECT_OPTION GetSelectedObj() { return m_eSelctedObj; }

private:
	// 드래그로 Wall을 만드는 함수
	//void CreateWall(Vec2 _vStartPos, Vec2 _vEndPos);
	

public:
	CScene_Tool();
	~CScene_Tool();
};

