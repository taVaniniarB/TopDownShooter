#pragma once
#include "CObject.h"

class CTexture;

class CUI :
    public CObject
{
private:
    // 자신의 자식 UI를 관리
    vector<CUI*>    m_vecChildUI;
    CUI* m_pParentUI; // nullptr: 부모가 없는 최상위 UI
    Vec2            m_vFinalPos; // 찐 최종 위치

    CTexture* m_pTex;

    bool            m_bCamAffected; // UI의 카메라 영향 받음 유뮤
    bool            m_bMouseOn;     // UI 위에 마우스 있다
    bool            m_bLbtnDown;    // UI에 왼쪽 버튼 눌린 적 있다


public:
    Vec2 GetFinalPos() { return m_vFinalPos; }
    CUI* GetParent() { return m_pParentUI; }
    bool IsMouseOn() { return m_bMouseOn; }
    bool IsLbtnDown() { return m_bLbtnDown; }

    

    // 자식 UI 들어오면 벡터에 넣고, 부모 연결
    void AddChild(CUI* _pUI)
    {
        m_vecChildUI.push_back(_pUI);
        _pUI->m_pParentUI = this;
    }

    // 안에 있는 데이터를 수정 안 할 거고 보기만 할 것 > const
    const vector<CUI*>& GetChildUI() { return m_vecChildUI; }

public:
    virtual void update();
    virtual void render(HDC _dc);
    virtual void finalUpdate();

private:
    void update_child();
    void finalUpdate_Child();
    void render_child(HDC _dc);

    void MouseOnCheck();

public:
    // 동작에 따른 기능 정의
    virtual void MouseOn(); // 마우스 갖다댐
    virtual void MouseLbtnDown(); // 왼쪽 버튼 눌렸을 때
    virtual void MousebtnUp(); // 마우스 떼졌을 때
    virtual void MouseLbtnClicked(); // 클릭

    virtual CUI* Clone() = 0;

public:
    // 카메라 영향 유무 까먹고 세팅 안 할 수 있으므로
    // UI 생성 시, 무조건 카메라 영향 여부를 받기로 하자 (기본생성자 없애기)
    CUI(bool _bCamAff);
    // 깊복 구현 위한 복.생
    CUI(const CUI& _origin);
    // 그냥 명시적으로 virtual 붙여줌
    virtual ~CUI();

    friend class CUIMgr;
};
