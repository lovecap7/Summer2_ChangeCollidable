#pragma once
#include "EventArea.h"
class Actor;
class ActorManager;
class Collidable;
class Camera;
class BossArea :
    public EventArea
{
public:
    BossArea(std::weak_ptr<Actor> start, std::weak_ptr<Actor> end);
    ~BossArea();
    void Init()override {};
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    void OnCollide(const std::shared_ptr<Collidable> other)override {};
    void Draw()const override {};
    void Complete() override {};
    void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override {};
    void End()override;
    bool IsEntryBossArea()const { return m_isEntryBossArea; };
private:
    //��ԑJ��
    using UpdateFunc_t = void(BossArea::*)(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
    UpdateFunc_t m_update;
    //�͈̓`�F�b�N���
    void EntryCheckUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
    //�C�x���g���
    void EventUpdate(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager);
    //�{�X�G���A�t���O
    bool m_isEntryBossArea;
};

