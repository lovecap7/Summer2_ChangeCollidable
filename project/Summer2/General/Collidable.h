#pragma once
#include <memory>
#include "Math/MyMath.h"
#include "Collision/ColliderBase.h"

//状態に合わせて当たり判定を行う時に使う
enum class State
{
	None,
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

enum class GameTag
{
	None,
	Player,		//プレイヤー
	Enemy,		//敵
	Item,		//アイテム
	Object,	//障害物
	Field,		//フィールド
};


class ColliderBase;
class Actor;
class Rigidbody;
class Collidable abstract
{
public:
	Collidable(State state, Priority priority, GameTag gameTag, Shape shape, bool isTrigger);
	virtual ~Collidable();
private:
	//コライダーの作成
	std::shared_ptr<ColliderBase> CreateCollider(Shape shape);
protected:
	//当たり判定
	std::shared_ptr<ColliderBase> m_coll;
	//座標とベクトル
	std::shared_ptr<Rigidbody> m_rb;
	//トリガー
	bool m_isTrigger;
	//状態
	State m_state;
	//優先度
	Priority m_priority;
	//タグ
	GameTag m_tag;
	//衝突判定を無視するか
	bool m_isThrough;
};



