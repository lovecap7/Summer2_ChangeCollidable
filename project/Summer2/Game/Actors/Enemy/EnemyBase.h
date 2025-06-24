#pragma once
#include "../Actor.h"
#include <memory>
#include "../../../General/Model.h"
class Trigger;
class AttackBase;
class AttackManager;
class SearchTrigger;
class EnemyBase abstract :
    public Actor
{
public:
    EnemyBase();
    virtual ~EnemyBase() {};
    //プレイヤーへのベクトルを返す関数
    Vector3 GetPlayerVec() const;
	//プレイヤーの位置をXZ平面上のベクトルにして返す関数
    Vector3 GetPlayerNomVecXZ() const;
    //索敵範囲
    std::shared_ptr<SearchTrigger> GetSearchTrigger() { return m_searchTrigger; };
    //モデルクラス
    std::shared_ptr<Model> GetModel() const { return m_model; };
protected:
    //モデル
    std::shared_ptr<Model> m_model;
    //索敵トリガー
    std::shared_ptr<SearchTrigger> m_searchTrigger;
    //やられ判定の更新
    virtual void UpdateHurtPoint()abstract;
    //攻撃判定を出す
    void AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager> attackManager);
};

