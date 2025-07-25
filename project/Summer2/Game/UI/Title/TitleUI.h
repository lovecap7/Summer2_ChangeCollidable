#pragma once
#include "../UIBase.h"
#include <memory>

class TitleUI :
    public UIBase
{
public:
    TitleUI();
    ~TitleUI();
    void Update()override;
    void Draw() const override;
private:
    //フレームを数える
    int m_countFrame;
    //決定
    bool m_isDecide;
	//出現処理が終わったかどうか
    bool m_isAppeared;
    //ハンドル
    int m_titleHandle;
    int m_titleBackHandle;
    int m_pressAnyBottunHandle;
    int m_pressAnyBottunBackHandle;
    //ディゾルブに使う変数
    //パターン
	int m_patternHandle;
    //シェーダ
	int m_pixelShaderHandle;
	//シェーダの定数バッファ
	int m_constantBufferHandle = -1;
	//シェーダのパラメータ
	float* m_threshold; 
    float m_dissolveRate;
};

