#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"

class ActorManager;
class Input;
class Camera;
class Collidable;
class HurtPoint;
/// <summary>
/// ゲーム中に配置可能な物体の基底クラス
/// </summary>
/// //newできなくなる
class Actor abstract
{
protected:
	//当たり判定や座標を持ったクラス
	std::shared_ptr<Collidable> m_collidable;
	//やられ判定
	std::shared_ptr<HurtPoint> m_hurtPoint;
	//消滅
	bool m_isDelete;
	//アクターの識別番号
	int m_id;
	//IDがセットされたことを記録するフラグ
	bool m_isSetId;
	//描画用のクラスかどうか
	bool m_isDrawOnly;
public:
	Actor();
	virtual ~Actor() {};
	//(純粋仮想関数)
	/// <summary>
	/// 登録処理
	/// </summary>
	virtual void Entry(std::shared_ptr<ActorManager> actorManager)abstract;
	/// <summary>
	/// 登録解除
	/// </summary>
	/// <param name="actorManager"></param>
	virtual void Exit(std::shared_ptr<ActorManager> actorManager)abstract;
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init()abstract;
	/// <summary>
	/// 位置などの更新を行う
	/// </summary>
	/// <param name="">入力オブジェクト</param>
	virtual void Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<ActorManager> actorManager) abstract;
	/// <summary>
	/// 重力を受ける
	/// </summary>
	/// <param name="gravity"></param>
	virtual void Gravity(const Vector3& gravity)abstract;
	/// <summary>
	/// 衝突したときのイベント関数
	/// </summary>
	/// <param name="other"></param>
	virtual void OnHitColl(const std::shared_ptr<Collidable>& other)abstract;
	/// <summary>
	/// 対象のアクターを表示
	/// </summary>
	virtual void Draw()const abstract;
	/// <summary>
	/// Updateの処理によって起こった衝突処理などの処理の確定
	/// </summary>
	virtual void Complete() abstract;
	/// <summary>
	/// 当たり判定と衝突処理に使う
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<Collidable> GetCollidable() const { return m_collidable; }
	/// <summary>
	/// やられ判定
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<HurtPoint> GetHurtPoint() const { return m_hurtPoint; }
	/// <summary>
	/// 削除フラグ
	/// </summary>
	/// <returns></returns>
	bool IsDelete() { return m_isDelete; };
	/// <summary>
	/// 削除予約
	/// </summary>
	/// <returns></returns>
	void Delete() { m_isDelete = true; };
	/// <summary>
	/// IDを取得
	/// </summary>
	/// <returns></returns>
	int GetID() const { return m_id; };
	/// <summary>
	/// IDをセット
	/// </summary>
	/// <returns></returns>
	void SetID(int id);
	/// <summary>
	/// 描画用クラス
	/// </summary>
	/// <returns></returns>
	bool IsDrawOnly() { return m_isDrawOnly; };
};

