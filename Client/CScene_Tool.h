#pragma once

#include "CScene.h"
class CScene_Tool:
	public CScene
{
private:
public:
	virtual void Enter();
	virtual void Exit();
	virtual void update();
	//virtual void finalUpdate();
	//virtual void render();

	// 작업 형태에 따라 분기처리 or Enum 정의해두자

public:
	// 마우스 클릭 시 타일 인덱스 1 증가
	void SetTileIdx();


public:
	CScene_Tool();
	~CScene_Tool();
};

