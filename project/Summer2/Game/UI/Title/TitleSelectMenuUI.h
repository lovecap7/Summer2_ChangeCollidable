#pragma once
#include "../UIBase.h"
#include "../../General/Math/MyMath.h"
#include <string>
class TitleSelectMenuUI :
    public UIBase
{
public:
    TitleSelectMenuUI(Vector2 pos,const std::wstring& text);
    ~TitleSelectMenuUI();
    void Update()override;
    void Draw() const override;
    void SetIsSelect(bool isSelect) { m_isSelect = isSelect; };
private:
    //自分の位置
    Vector2 m_pos;
    //初期位置
    Vector2 m_firstPos;
    //選ばれたときの位置
    Vector2 m_selectPos;
    //選ばれてるか
    bool m_isSelect;
    //ハンドル
    int m_textHandle;
    //テキスト
    std::wstring m_text;
};

