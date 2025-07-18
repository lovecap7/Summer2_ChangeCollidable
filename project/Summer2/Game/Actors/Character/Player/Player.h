#pragma once
#include "../CharacterBase.h"
#include "../../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class UltGage;
class Camera;
class CharacterStateBase;
class HitPoints;
class TrackActorEffect;
class Player :
	public CharacterBase
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	//初期化処理
	void Init()override;
	//更新処理
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//衝突イベント関数
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//描画
	void Draw()const override;
	//更新処理による結果の確定
	void Complete() override;
	//死亡処理
	void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override;
	//終了処理
	void End()override;
	//入力中の方向キー
	Vector2 GetStickVec() { return m_stickVec; };
	//必殺技ゲージ
	std::weak_ptr<UltGage> GetUltGage() const{ return m_ultGage; };
	//リジッドボディ
	std::shared_ptr<Rigidbody> GetRb() const { return m_rb; }
	//コリジョン
	std::shared_ptr<ColliderBase> GetColl() const { return m_collisionData; }
	//コリジョンの状態を設定
	void SetCollState(CollisionState collState) { m_collState = collState; }
	//ターゲットのデータ
	TargetData GetTargetData() const { return m_targetData; };
	//ダッシュキープフラグ
	bool IsRunKeep() { return m_isRunKeep; };
	void SetIsRunKeep(bool isRunKeep) { m_isRunKeep = isRunKeep; };
	//ゲームクリアアニメーション終了
	bool IsFinishClearAnim();
	//ターゲットの索敵
	virtual void TargetSearch(float searchDistance, float searchAngle, Vector3 targetPos) override;
private:
	//スティックの向きを持つベクトル
	Vector2 m_stickVec;
	//必殺技ゲージ
	std::shared_ptr<UltGage> m_ultGage;
	//ダッシュ状態をキープするフラグ
	bool m_isRunKeep;
	//ダッシュ状態を解除するフレーム
	int m_cancelRunFrame;
	//必殺ゲージMAXエフェクト
	std::weak_ptr<TrackActorEffect> m_ultMaxEff;

	//落下したときに戻す座標
	Vector3 m_initPos;
private:
	//走りを継続するか
	void CheckRunKeep();
	//必殺ゲージが最大か確認
	void CheckUltMax();
};

