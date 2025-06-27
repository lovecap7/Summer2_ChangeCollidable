#include "PlayerStateChargeAttackBase.h"
class Actor;
class Camera;
class ActorManager;
class PlayerStateCA2 :
    public PlayerStateChargeAttackBase, public std::enable_shared_from_this<PlayerStateCA2>
{
public:
    PlayerStateCA2(std::weak_ptr<Actor> player);
    ~PlayerStateCA2();
    void Init() override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};



