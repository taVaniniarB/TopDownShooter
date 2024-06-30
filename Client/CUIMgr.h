#pragma once

class CUI;

class CUIMgr
{
	SINGLE(CUIMgr);

private:
	CUI*	m_pFocusedUI;

public:
	void update();
	// 단축키 등을 통해 바로 포커싱 해주는 함수
	void SetFocusedUI(CUI* _pUI);

private:
	CUI* GetFocusedUI();
	// 부모 UI에서 실제로 타겟팅된 UI 찾아서 변환한다.
	CUI* GetTargetedUI(CUI* _pParentUI);
};

