#pragma once
#include "AttackBase.h"
#include "../../General/Math/Vector3.h"
class BulletAttack :
	public AttackBase, public std::enable_shared_from_this<BulletAttack>
{
public:
	/// <summary>
	/// 遠距離攻撃
	/// </summary>
	/// <param name="coll">衝突確認のための情報</param>
	/// <param name="damage">ダメージ</param>
	/// <param name="keepFrame">持続</param>
	/// <param name="knockBack">ノックバックの大きさ</param>
	/// <param name="owner">持ち主</param>
	BulletAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, float knockBackPower, std::shared_ptr<Actor> owner);
	~BulletAttack() override;
	//初期化
	void Init() override;
	//更新処理
	void Update() override;
	//描画
	void Draw() override;
	//当たった時
	void OnHit(std::shared_ptr<Actor> actor) override;

	//方向と速度を設定
	void SetDirAndSpeed(const Vector3& dir, float speed)
	{
		m_dir = dir;
		m_speed = speed;
	}

private:
	//方向
	Vector3 m_dir;
	//移動速度
	float m_speed = 0.0f;
};

