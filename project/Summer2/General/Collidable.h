#pragma once
#include <memory>
#include "Math/MyMath.h"
#include "Collision/ColliderBase.h"


//状態に合わせて当たり判定を行う時に使う
enum class CollisionState
{
	Normal,
	Jump,
	Fall,
	Dead
};

// 位置補正の優先度の判別に使う
enum class Priority : int
{
	Low,		// 低
	Middle,		// 中
	High,		// 高
	Static,		// 動かない（最高）
};

// ゲーム内のタグ付け
enum class GameTag
{
	None,
	Player,		//プレイヤー
	Enemy,		//敵
	Item,		//アイテム
	Object,		//障害物
	Field,		//フィールド
};

//レイヤー
enum class Layer
{
	Actor,		//アクター
	Effect,		//エフェクト
	Attack,		//攻撃
	HurtPoint,	//やられ判定
};


class ColliderBase;
class Actor;
class Rigidbody;
class Physics;
class CollisionChecker;
class CollisionProcess;
class Collidable abstract : 
	public std::enable_shared_from_this<Collidable>
{
public:
	Collidable(Shape shape);
	virtual ~Collidable() {};

	/// <summary>
	/// 衝突したときのイベント関数
	/// </summary>
	/// <param name="other"></param>
	virtual void OnCollide(const std::shared_ptr<Collidable> other)abstract;
	/// <summary>
	/// Updateの処理によって起こった衝突処理などの処理の確定
	/// </summary>
	virtual void Complete() abstract;
	
	/// <summary>
	/// Physicsに登録
	/// </summary>
	virtual void Init();
	/// <summary>
	/// Physicsから解除
	/// </summary>
	virtual void End();
	//タグ
	GameTag GetGameTag() { return m_tag; };
	void SetGameTag(GameTag gameTag) { m_tag = gameTag; };
	//コライダーの形
	Shape GetShape() const { return m_collisionData->m_shape; };

	//地面に当たったかどうか
	bool IsFloor() { return m_isFloor; };
	bool IsWall() { return m_isWall; };
	void SetIsFloor(bool isFloor) { m_isFloor = isFloor; };
	void SetIsWall(bool isWall) { m_isWall = isWall; };
	//床と壁のフラグ
	void ResetHitFlag() { m_isFloor = m_isWall = false; };
private:
	void CreateCollider(Shape shape);
	//PhysicsがCollidableを自由に管理できるように
	friend Physics;
	friend CollisionChecker;
	friend CollisionProcess;
protected:
	//当たり判定
	std::shared_ptr<ColliderBase> m_collisionData;
	//座標とベクトル
	std::shared_ptr<Rigidbody> m_rb;
	//衝突判定を行わない(trueなら)
	bool m_isThrough;
	//トリガー
	bool m_isTrigger;
	//状態
	CollisionState m_collState;
	//優先度
	Priority m_priority;
	//タグ
	GameTag m_tag;
	//床に当たったならtrue
	bool m_isFloor;
	//壁に当たったならtrue
	bool m_isWall;
};



