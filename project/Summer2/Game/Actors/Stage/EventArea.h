#pragma once
#include "../Actor.h"
#include <list>
//�v���C���[���͈͓��ɓ������������m���C�x���g���I���܂Ń`�F�b�N����
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
    //��ԑJ��
    using UpdateFunc_t = void(EventArea::*)(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
    UpdateFunc_t m_update;
    //�͈̓`�F�b�N���
    void EntryCheckUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
    //�C�x���g���
    void EventUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
};

