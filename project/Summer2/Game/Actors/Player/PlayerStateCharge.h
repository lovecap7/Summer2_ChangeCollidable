#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class ActorkManager;
class PlayerStateCharge :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateCharge>
{
public:
    PlayerStateCharge(std::shared_ptr<Player> player);
    ~PlayerStateCharge();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //ƒ^ƒŠÔ
    int m_chargeFrame;
    //Œ¸‘¬
    void SpeedDown();
};



