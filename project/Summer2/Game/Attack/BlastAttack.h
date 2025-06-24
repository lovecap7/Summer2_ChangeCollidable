#pragma once
#include "AttackBase.h"
#include "../../General/Math/Vector3.h"
class BlastAttack :
    public AttackBase, public std::enable_shared_from_this<BlastAttack>
{
public:
	/// <summary>
	/// 範囲攻撃
	/// </summary>
	/// <param name="coll">衝突確認のための情報</param>
	/// <param name="damage">ダメージ</param>
	/// <param name="keepFrame">持続</param>
	/// <param name="knockBack">ノックバックの大きさ</param>
	/// <param name="owner">持ち主</param>
	BlastAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, float knockBackPower, std::shared_ptr<Actor> owner);
	~BlastAttack() override;
	//初期化
	void Init() override;
	//更新処理
	void Update() override;
	//描画
	void Draw() override;
	//当たった時
	void OnHit(std::shared_ptr<Actor> actor) override;
	//吹き飛ばす方向
	Vector3 GetKnockBackVec(Vector3 other) override;
};
