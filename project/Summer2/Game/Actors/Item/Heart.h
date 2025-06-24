#pragma once
#include "ItemBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class Input;
class Camera;
class Model;
class AttackManager;
class UIManager;
class Heart :
	public ItemBase, public std::enable_shared_from_this<Heart>
{
public:
	Heart(int modelHandle, Vector3 pos);
	virtual ~Heart();
	//登録処理
	void Entry(std::shared_ptr<ActorManager> actorManager)override;
	//登録解除
	void Exit(std::shared_ptr<ActorManager> actorManager)override;
	//初期化処理
	void Init()override;
	//更新処理
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
	//重力
	void Gravity(const Vector3& gravity)override;
	//衝突イベント関数
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	//描画
	void Draw()const override;
	//更新処理による結果の確定
	void Complete() override;
private:
	//モデル
	std::shared_ptr<Model> m_model;
	//最初の数フレームは当たり判定をしたくないので
	int m_noHitFrame;
};

