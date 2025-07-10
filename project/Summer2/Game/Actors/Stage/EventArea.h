#pragma once
#include "../Actor.h"
#include <list>
//プレイヤーが範囲内に入ったかを検知しイベントが終了までチェックする
class Camera;
class ActorManager;
class EnemyBase;
class EventArea:
	public Actor
{
public:
	EventArea(std::weak_ptr<Actor> start, std::weak_ptr<Actor> end);
    virtual ~EventArea();
    virtual void Init()override {};
    virtual void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    virtual void OnCollide(const std::shared_ptr<Collidable> other)override {};
    virtual void Draw()const override {};
    virtual void Complete() override {};
    virtual void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override {};
    virtual void End()override;
    //イベント中
    bool IsEvent()const { return m_isEvent; };
    //スタートとエンドのX座標
    Vector3 GetEventStartPos()const { return m_start.lock()->GetPos(); };
    Vector3 GetEventEndPos()const { return m_end.lock()->GetPos(); };
protected:
    std::weak_ptr<Actor> m_start;
    std::weak_ptr<Actor> m_end;
    //イベント中フラグ
    bool m_isEvent;
private:
    //状態遷移
    using UpdateFunc_t = void(EventArea::*)(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
    UpdateFunc_t m_update;
    //範囲チェック状態
    void EntryCheckUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
    //イベント状態
    void EventUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
    //範囲内の敵のリスト
    std::list<std::weak_ptr<EnemyBase>> m_areaEnemies;
};

