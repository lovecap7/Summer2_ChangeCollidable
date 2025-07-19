#pragma once
#include <map>
#include <string>
/// <summary>
/// テキストデータを管理するシングルトンクラス
/// </summary>
class TextManager final
{
private:
	//シングルトンの準備
	TextManager() = default;
	~TextManager() = default;
	//コピー禁止
	TextManager(const TextManager&) = delete;
	TextManager& operator = (const TextManager&) = delete;
	//ムーブ禁止
	TextManager(TextManager&&) = delete;
	TextManager& operator = (TextManager&&) = delete;
public:
	//インスタンスを取得
	static TextManager& GetInstance()
	{
		static TextManager instance;
		return instance;
	}
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw()const;
	//終了処理
	void End();
private:
	//ハンドル
	std::map<std::string, int> m_handles;
};

