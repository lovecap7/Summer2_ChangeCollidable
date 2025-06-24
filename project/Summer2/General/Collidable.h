#pragma once
#include <memory>
#include "Math/MyMath.h"

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
class Collidable
{
public:
	Collidable(std::shared_ptr<ColliderBase> coll, std::shared_ptr<Rigidbody> rb);
	virtual ~Collidable() {};
	//初期化処理
	void Init(State state, Priority priority, GameTag gameTag);
	//当たり判定
	const std::shared_ptr<ColliderBase>& GetColl() const { return m_coll; }
	//座標とベクトル
	const std::shared_ptr<Rigidbody>& GetRb()const { return m_rb; }
	//当たり判定を行うかどうか
	bool IsCollide() const { return m_isCollide; };
	void SetIsCollide(bool isCollide) { m_isCollide = isCollide; }
	//状態
	State GetState() { return m_state; };
	void SetState(State state) { m_state = state; };
	//持ち主
	std::shared_ptr<Actor> GetOwner() { return m_owner; };
	void SetOwner(std::shared_ptr<Actor> owner) { m_owner = owner; };
	//優先度
	Priority GetPriority() { return m_priority; };
	void SetPriority(Priority priority) { m_priority = priority; };
	//タグ
	GameTag GetGameTag() { return m_tag; };
	void SetGameTag(GameTag gameTag) { m_tag = gameTag; };
private:
	//当たり判定
	std::shared_ptr<ColliderBase> m_coll;
	//座標とベクトル
	std::shared_ptr<Rigidbody> m_rb;
	//衝突判定を行う
	bool m_isCollide;
	//状態
	State m_state;
	//持ち主
	std::shared_ptr<Actor> m_owner;
	//優先度
	Priority m_priority;
	//タグ
	GameTag m_tag;
};



