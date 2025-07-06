#pragma once

/// <summary>
/// シングルトン
/// </summary>
class Application
{
private:
	//シングルトンの準備
	Application() = default;
	~Application() = default;
	//コピー禁止
	Application(const Application&) = delete;
	Application& operator = (const Application&) = delete;
	//ムーブ禁止
	Application(Application&&) = delete;
	Application& operator = (Application&&) = delete;
public:
	/// <summary>
	/// シングルトンオブジェクトを返す関数
	/// 参照を返すことで、複製を防ぐ
	/// 間違えて値を返した場合、コピーコンストラクタが走るので
	/// それをprivateにしておくことで弾くことができる
	/// </summary>
	/// <returns></returns>
	static Application& GetInstance();

	/// <summary>
	/// アプリケーションの初期化
	/// </summary>
	/// <returns>ture:初期化成功, false:失敗</returns>
	bool Init();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Run();
	/// <summary>
	/// 終了処理
	/// </summary>
	void Terminate();
};
