#pragma once
#include "CUI.h"
// � ��ü�� ���¸� ǥ���ϴ� UI (HP(�÷��̾�), �ܿ� �Ѿ� ��(�÷��̾ ��� �ִ� ��), ����Ʈ(����Ʈ�Ŵ���), �޺�((����Ʈ�Ŵ���)))
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

