#include "PlayerStateBase.h"
class Actor;
class Camera;
class ActorManager;
class PlayerStateCA1 :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateCA1>
{
public:
    PlayerStateCA1(std::weak_ptr<Actor> player);
    ~PlayerStateCA1();
    void Init() override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
private:
};



