#pragma once
#include "CUI.h"
class CPanelUI :
    public CUI
{
private:
    Vec2    m_vDragStart;
public:
    virtual void update();
    virtual void render(HDC _dc);
    virtual void MouseOn(); // 마우스 갖다댐
    virtual void MouseLbtnDown(); // 왼쪽 버튼 눌렸을 때
    virtual void MousebtnUp(); // 마우스 떼졌을 때

    CLONE(CPanelUI);

public:
    CPanelUI();
    ~CPanelUI();
};

