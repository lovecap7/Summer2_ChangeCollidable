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
	~EventArea();
    void Init()override {};
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    void OnCollide(const std::shared_ptr<Collidable> other)override {};
    void Draw()const override {};
    void Complete() override {};
    void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override {};
    void End()override;
private:
    std::weak_ptr<Actor> m_start;
    std::weak_ptr<Actor> m_end;
    std::list<std::weak_ptr<EnemyBase>> m_areaEnemies;
    //状態遷移
    using UpdateFunc_t = void(EventArea::*)(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
    UpdateFunc_t m_update;
    //範囲チェック状態
    void EntryCheckUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
    //イベント状態
    void EventUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
};

