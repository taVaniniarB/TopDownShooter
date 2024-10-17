#pragma once
#include "CUI.h"
class CTextUI :
    public CUI
{
public:
    CTextUI();
    ~CTextUI();

    CLONE(CTextUI);

    void render(HDC _dc);

private:
    wstring m_strText;
    FONT_TYPE m_eFont;

public:
    void SetText(wstring _strText) { m_strText = _strText; }
    void SetText(int _iNum) { m_strText = std::to_wstring(_iNum); }
    void SetFont(FONT_TYPE _eFont) { m_eFont = _eFont; }

    wstring GetText() { return m_strText; }
    FONT_TYPE GetFont() { return m_eFont; }
};