#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
#include "../../General/Collision/Collidable.h"

class ActorManager;
class Camera;
class Collidable;
class Model;
/// <summary>
/// ゲーム中に配置可能な物体の基底クラス
/// </summary>
/// //newできなくなる
class Actor abstract:
	public Collidable
{
protected:
	//消滅
	bool m_isDelete;
	//アクターの識別番号
	int m_id;
	//IDがセットされたことを記録するフラグ
	bool m_isSetId;
	//モデル
	std::shared_ptr<Model> m_model;
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
	virtual void Update(const std::weak_ptr<Camera> camera,const std::weak_ptr<ActorManager> actorManager) abstract;
	/// <summary>
	/// 対象のアクターを表示
	/// </summary>
	virtual void Draw()const abstract;
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
	/// 座標
	/// </summary>
	Vector3 GetPos() const;
	//モデルクラス
	std::shared_ptr<Model> GetModel() const { return m_model; };
};

