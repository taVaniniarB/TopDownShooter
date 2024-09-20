#pragma once
#include "CUI.h"
// 어떤 객체의 상태를 표시하는 UI (HP(플레이어), 잔여 총알 수(플레이어가 들고 있는 총), 포인트(포인트매니저), 콤보((포인트매니저)))
class CStatUI :
    public CUI
{
public:
    CStatUI();
    ~CStatUI();

    CLONE(CStatUI);

private:
    CObject* m_pTarget;
};

