#include "TextManager.h"
#include <DxLib.h>
#include <cassert>

//初期化
void TextManager::Init()
{
	m_handles["メイリオ"] = CreateFontToHandle("Meiryo（メイリオ）", 32, 5, DX_FONTTYPE_ANTIALIASING);
	//ロードに成功したかチェック
	for (auto& [key, value] : m_handles) {
		assert(value >= 0);
	}
}

//更新
void TextManager::Update()
{

}
//描画
void TextManager::Draw()const
{
	DrawStringToHandle(100, 500, "ラノベ風のセリフ表示", 0xffffff, m_handles["メイリオ"]);
}
//終了
void TextManager::End()
{
	//ロードに成功したかチェック
	for (auto& [key, value] : m_handles) {
		DeleteFontToHandle(value);
	}
	m_handles.clear();
}