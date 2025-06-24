#pragma once
#include "ItemBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class Input;
class Camera;
class Model;
class BlastAttack;
class Bomb :
	public ItemBase, public std::enable_shared_from_this<Bomb>
{
public:
	Bomb(int modelHandle, Vector3 pos,Vector3 vec);
	virtual ~Bomb();
	//登録処理
	void Entry(std::shared_ptr<ActorManager> actorManager)override;
	//登録解除
	void Exit(std::shared_ptr<ActorManager> actorManager)override;
	//初期化処理
	void Init()override;
	//更新処理
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
	void BombAnim();
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
	//攻撃
	std::shared_ptr<BlastAttack> m_blastAttack;
	//爆発のカウント
	int m_blastCountFrame;
	//モデルのサイズ
	Vector3 m_originalScale;//元のモデルのサイズを保持
	//モデルの大きさをcosを使って変更するので角度を保持
	float m_scaleSpeed;
private:
	//爆発の当たり判定作成
	void CreateAttack();
	//減速
	void SpeedDown();
};

