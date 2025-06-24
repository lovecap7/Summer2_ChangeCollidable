#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class ActorManager;
class MeleeAttack;
class Collidable;
class PlayerStateCA1 :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateCA1>
{
public:
    PlayerStateCA1(std::shared_ptr<Player> player, const std::shared_ptr<ActorManager> attackManager);
    ~PlayerStateCA1();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //¶‘«
    std::shared_ptr<Collidable> m_leftLeg;
    //UŒ‚”»’è
    std::shared_ptr<MeleeAttack> m_attackC;
    //UŒ‚”»’è‚ğì¬
    void CreateAttack();
    //UŒ‚‚Ìó‘ÔXV
    void UpdateAttack();
    //Œ¸‘¬
    void SpeedDown();
    //íœ
    void DeleteAttack(const std::shared_ptr<AttackManager> attackManager);
};



