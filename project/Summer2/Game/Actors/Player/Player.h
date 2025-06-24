#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class Input;
class Camera;
class Model;
class AttackBase;
class MeleeAttack;
class AttackManager;
class UIManager;
class UltGage;
class SearchTrigger;
class PlayerStateBase;
class Player :
	public Actor, public std::enable_shared_from_this<Player>
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	//登録処理
	void Entry(std::shared_ptr<ActorManager> actorManager)override;
	//登録解除
	void Exit(std::shared_ptr<ActorManager> actorManager)override;
	//初期化処理
	void Init()override;
	//更新処理
	void Update(const Input& input,const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
	//重力
	void Gravity(const Vector3& gravity)override;
	//衝突イベント関数
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	//描画
	void Draw()const override;
	//更新処理による結果の確定
	void Complete() override;
	//地面に付いているか
	bool IsGround() { return m_isGround; };
	void NoIsGround() { m_isGround = false; };//地面に付いていないと判断したときに呼ぶ
	//入力中の方向キー
	Vector2 GetStickVec() { return m_stickVec; };
	//モデルクラス
	std::shared_ptr<Model> GetModel() const{ return m_model; };
	//必殺技ゲージ
	std::shared_ptr<UltGage> GetUltGage() const{ return m_ultGage; };
	//索敵
	std::shared_ptr<SearchTrigger> GetSearchTrigger() { return m_searchTrigger; };
	//アイテム
	//ダメージカット
	void SetDamageCut(float cutRate, int keepFrame);
private:
	//プレイヤーの状態
	std::shared_ptr<PlayerStateBase> m_state;
	//モデル
	std::shared_ptr<Model> m_model;
	//スティックの向きを持つベクトル
	Vector2 m_stickVec;
	//地面に付いているかどうか
	bool m_isGround;
	//必殺技ゲージ
	std::shared_ptr<UltGage> m_ultGage;
	//索敵トリガー
	std::shared_ptr<SearchTrigger> m_searchTrigger;
	//ダメージカットフレーム
	int m_damageCutFrame;
	//ダメージアップ率
	float m_damageUpRate;
	//ダメージアップの持続フレーム
	int m_damageUpKeepFrame;	
private:
	//やられ判定の更新
	void UpdateHurtPoint();
};

