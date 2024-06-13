#pragma once
#include "CObject.h"

class CUI :
    public CObject
{
private:
    // �ڽ��� �ڽ� UI�� ����
    vector<CUI*>    m_vecChildUI;
    CUI*            m_pParentUI; // nullptr: �θ� ���� �ֻ��� UI
    Vec2            m_vFinalPos; // �� ���� ��ġ

    bool            m_bCamAffected; // UI�� ī�޶� ���� ���� ����
    bool            m_bMouseOn;     // UI ���� ���콺 �ִ�
    bool            m_bLbtnDown;    // UI�� ���� ��ư ���� �� �ִ�


public:
    Vec2 GetFinalPos() { return m_vFinalPos; }
    CUI* GetParent() { return m_pParentUI; }
    bool IsMouseOn() { return m_bMouseOn; }

    // �ڽ� UI ������ ���Ϳ� �ְ�, �θ� ����
    void AddChild(CUI* _pUI)
    {
        m_vecChildUI.push_back(_pUI);
        _pUI->m_pParentUI = this;
    }

    // �ȿ� �ִ� �����͸� ���� �� �� �Ű� ���⸸ �� �� > const
    const vector<CUI*>& GetChildUI() { return m_vecChildUI; }

public:
    virtual void update();
    virtual void render(HDC _dc);
    virtual void finalUpdate();

    CLONE(CUI);

private:
    void update_child();
    void finalUpdate_Child();
    void render_child(HDC _dc);

    void MouseOnCheck();

public:
    // ���ۿ� ���� ��� ����
    virtual void MouseOn(); // ���콺 ���ٴ�
    virtual void MouseLbtnDown(); // ���� ��ư ������ ��
    virtual void MousebtnUp(); // ���콺 ������ ��
    virtual void MouseLbtnClicked(); // Ŭ��

public:
    // ī�޶� ���� ���� ��԰� ���� �� �� �� �����Ƿ�
    // UI ���� ��, ������ ī�޶� ���� ���θ� �ޱ�� ���� (�⺻������ ���ֱ�)
    CUI(bool _bCamAff);
    // �׳� ���������� virtual �ٿ���
    virtual ~CUI();

    friend class CUIMgr;
};
