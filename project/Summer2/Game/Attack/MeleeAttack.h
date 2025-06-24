#pragma once
#include "AttackBase.h"
class MeleeAttack :
    public AttackBase, public std::enable_shared_from_this<MeleeAttack>
{
public:
	/// <summary>
	/// 近接攻撃
	/// </summary>
	/// <param name="coll">衝突確認のための情報</param>
	/// <param name="damage">ダメージ</param>
	/// <param name="keepFrame">持続</param>
	/// <param name="knockBack">ノックバックの大きさ</param>
	/// <param name="owner">持ち主</param>
	MeleeAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, float knockBackPower, std::shared_ptr<Actor> owner);
	~MeleeAttack() override;
	//初期化
	void Init() override;
	//更新処理
	void Update() override;
	//描画
	void Draw() override;
	//当たった時
	void OnHit(std::shared_ptr<Actor> actor) override;
};

