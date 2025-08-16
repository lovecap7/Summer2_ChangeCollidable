#pragma once
#include "UIBase.h"
#include "../../General/Math/MyMath.h"
#include <string>
class MenuUI :
    public UIBase
{
public:
    MenuUI(Vector2 pos, int handle);
    ~MenuUI();
    void Update()override;
    void Draw() const override;
    void SetIsSelect(bool isSelect) { m_isSelect = isSelect; };
    bool IsSelect() const{ return m_isSelect; };
private:
    //自分の位置
    Vector2 m_pos;
    //選ばれてるか
    bool m_isSelect;
    //ハンドル
    int m_handle;
    //サイズ
    float m_size;
};

