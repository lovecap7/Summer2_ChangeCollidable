#pragma once
#include <memory>
#include <list>
class MyEffect;
/// <summary>
/// エフェクトを管理するシングルトンクラス
/// </summary>
class EffekseerManager final
{
private:
	//シングルトンの準備
	EffekseerManager() = default;
	~EffekseerManager() = default;
	//コピー禁止
	EffekseerManager(const EffekseerManager&) = delete;
	EffekseerManager& operator = (const EffekseerManager&) = delete;
	//ムーブ禁止
	EffekseerManager(EffekseerManager&&) = delete;
	EffekseerManager& operator = (EffekseerManager&&) = delete;
public:
	//インスタンスを取得
	static EffekseerManager& GetInstance()
	{
		static EffekseerManager instance;
		return instance;
	}
	//登録
	void Entry(std::shared_ptr<MyEffect> eff);
	//解除
	void Exit(std::shared_ptr<MyEffect> eff);
	//初期化
	void Init();
	//更新処理
	void Update();
	//描画
	void Draw()const;
	//終了処理
	void End();
private:
	//エフェクトの配列
	std::list<std::shared_ptr<MyEffect>> m_effects;
};

