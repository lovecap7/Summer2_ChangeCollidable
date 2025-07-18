#pragma once
//スマートポインタを使う
#include<memory>
#include<list>
class SceneBase;
/// <summary>
/// 各シーンを管理するクラス
/// 管理はするが、繊維を管理はしない(繊維はクラスどもがやる)
/// インターフェイスを提供するクラス
/// </summary>
class SceneController
{
public:
	SceneController();
	/// <summary>
	/// Applicationから呼ばれる関数。現在のシーンのUpdateをそのまま実行。
	/// </summary>
	
	void Update();
	/// <summary>
	/// Applicationから呼ばれる関数。現在のシーンのDrawをそのまま実行。
	/// </summary>
	/// <param name="input"></param>
	void Draw();
	/// <summary>
	/// 次の状態をセットする(セットするのは各状態の役割)
	/// 各シーンがコントローラーを見て切り替えさせる
	/// </summary>
	/// <param name="">次の状態のポインタ</param>
	void ChangeScene(std::shared_ptr<SceneBase> scene);
	/// <summary>
	/// 最初に入れたシーンを切り替える
	/// </summary>
	/// <param name=""></param>
	void ChangeBaseScene(std::shared_ptr<SceneBase> scene);
	/// <summary>
	/// シーンをプッシュ(入れる)する
	/// </summary>
	/// <param name=""></param>
	void PushScene(std::shared_ptr<SceneBase> scene);
	/// <summary>
	/// シーンをポップ(取り出す)する
	/// </summary>
	/// <param name=""></param>
	void PopScene();
	/// <summary>
	/// 最初に入れたシーンをもう一度初期の状態に戻す
	/// </summary>
	void RestartBaseScene();
	/// <summary>
	/// 最初に入れたシーンの更新
	/// </summary>
	void UpdateBaseScene();
	/// <summary>
	/// ロードシーンの準備
	/// </summary>
	/// <param name="scene"></param>
	void LoadAndChangeScene(std::shared_ptr<SceneBase> scene);
private:
	using SceneStack_t = std::list<std::shared_ptr<SceneBase>>;//シーンをスタック構造にするための型
	SceneStack_t m_scenes;//現在実行中のシーン(中が何かは知らない)
};

