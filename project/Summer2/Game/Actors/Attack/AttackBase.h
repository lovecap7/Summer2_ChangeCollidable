#pragma once
#include <list>
#include "../Actor.h"
#include "../../../General/Battle.h"

enum class AttackType
{
	Slash,
	Strike,
	AreaOfEffect,
	Bullet,
	Blast,
};

class CharacterStateBase;
class Actor;
class AttackBase abstract:
    public Actor 
{
public:
	AttackBase(Shape shape, std::weak_ptr<Actor> owner);
	virtual ~AttackBase();
	//初期化処理
	virtual void Init()override;
	//更新処理
	virtual void Update() ;
	//衝突イベント
	virtual void OnCollide(const std::shared_ptr<Collidable> other)override;
	//更新処理の確定
	void Complete() override {};
	//死亡処理
	void Dead(const std::weak_ptr<ActorManager> actorManager) override {};
	//ノックバック
	virtual Vector3 GetKnockBackVec(Position3 other);

	//まとめて設定
	void AttackSetting(int damage,int keepFrame,int knockBackPower, Battle::AttackWeight aw);
	//ダメージ
	int GetDamage();
	void SetDamage(int damage) { m_damage = damage; };
	//持続フレーム
	void SetKeepFrame(int keepFrame) { m_keepFrame = keepFrame; };
	//ノックバックの大きさ
	float GetKnockBackPower() { return m_knockBackPower; };
	void SetKnockBackPower(float knockBackPower) { m_knockBackPower = knockBackPower; };
	//攻撃の強さ
	Battle::AttackWeight GetAttackWeight();
	void SetAttackWeight(Battle::AttackWeight ap) { m_attackWeight = ap; };
	//当てたアクターのIDをリセット
	void ResetHitId() { m_hitId.clear(); };
	//攻撃を削除する
	void Delete() { m_isDelete = true; };
	//持ち主のタグ
	GameTag GetOwnerTag() { return m_ownerTag; };
protected:
	//ダメージ
	int m_damage;
	//持続フレーム
	int m_keepFrame;
	//ノックバックの大きさ
	float m_knockBackPower;
	//攻撃の強さ
	Battle::AttackWeight m_attackWeight;
	//当てたことのあるCollidableを覚えておく
	std::list<int> m_hitId;
	//持ち主の参照
	std::weak_ptr<Actor> m_owner;
	//持ち主のタグ
	GameTag m_ownerTag;
	
};

