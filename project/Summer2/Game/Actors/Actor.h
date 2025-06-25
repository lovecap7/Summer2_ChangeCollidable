#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
#include "../../General/Collidable.h"

class ActorManager;
class Input;
class Camera;
class Collidable;
class HurtPoint;
/// <summary>
/// ゲーム中に配置可能な物体の基底クラス
/// </summary>
/// //newできなくなる
class Actor abstract:
	public Collidable
{
protected:
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
	Actor(Shape shape);
	virtual ~Actor() {};
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init()abstract;
	/// <summary>
	/// 位置などの更新を行う
	/// </summary>
	/// <param name="">入力オブジェクト</param>
	virtual void Update(const std::weak_ptr<Camera> camera) abstract;
	/// <summary>
	/// 対象のアクターを表示
	/// </summary>
	virtual void Draw()const abstract;
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

