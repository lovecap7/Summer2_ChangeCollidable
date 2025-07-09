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
    virtual ~EventArea();
    virtual void Init()override {};
    virtual void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    virtual void OnCollide(const std::shared_ptr<Collidable> other)override {};
    virtual void Draw()const override {};
    virtual void Complete() override {};
    virtual void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override {};
    virtual void End()override;
protected:
    std::weak_ptr<Actor> m_start;
    std::weak_ptr<Actor> m_end;
private:
    //��ԑJ��
    using UpdateFunc_t = void(EventArea::*)(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
    UpdateFunc_t m_update;
    //�͈̓`�F�b�N���
    void EntryCheckUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
    //�C�x���g���
    void EventUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
    //�͈͓��̓G�̃��X�g
    std::list<std::weak_ptr<EnemyBase>> m_areaEnemies;
};

