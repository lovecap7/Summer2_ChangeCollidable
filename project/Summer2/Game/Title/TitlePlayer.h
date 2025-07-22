#pragma once
#include <memory>
class Model;
class TitlePlayer
{
public:
	TitlePlayer();
	~TitlePlayer();
	//初期化処理
	void Init();
	//更新処理
	void Update();
	//描画
	void Draw() const;
	//終了処理
	void End();
private:
	//モデル
	std::unique_ptr<Model> m_model;
};

