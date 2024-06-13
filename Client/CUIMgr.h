#pragma once

class CUI;

class CUIMgr
{
	SINGLE(CUIMgr);

private:
public:
	void update();

private:
	// 부모 UI에서 실제로 타겟팅된 UI 찾아서 변환한다.
	CUI* GetTargetedUI(CUI* _pParentUI);
};

