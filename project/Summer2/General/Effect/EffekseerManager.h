#pragma once
#include <memory>
#include <list>
#include <map>
#include <string>
#include "../Math/MyMath.h"

class MyEffect;
class Actor;
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
	//すべてのエフェクトを削除
	void AllDeleteEffect();
	//エフェクトを作成
	std::weak_ptr<MyEffect> CreateEffect(std::string name, Vector3 pos);
	std::weak_ptr<MyEffect> CreateTrackActorEffect(std::string name, std::weak_ptr<Actor> actor);
private:
	//ハンドル
	std::map<std::string, int> m_handles;
	//エフェクトの配列
	std::list<std::shared_ptr<MyEffect>> m_effects;
private:
	//ハンドルロード
	void LoadHandle();
	//ハンドルをすべて削除
	void AllDeleteHandle();
	//削除候補のエフェクトを削除
	void CheckDeleteEffect();
};

