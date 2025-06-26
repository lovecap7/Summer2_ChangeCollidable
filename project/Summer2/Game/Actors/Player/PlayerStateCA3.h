#include "PlayerStateBase.h"
class Player;
class Camera;
class PlayerStateCA3 :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateCA3>
{
public:
    PlayerStateCA3(std::weak_ptr<Player> player);
    ~PlayerStateCA3();
    void Init() override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
private:
  
};



