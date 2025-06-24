#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class Collidable;
class MeleeAttack;
class PlayerStateCA3 :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateCA3>
{
public:
    PlayerStateCA3(std::shared_ptr<Player> player, const std::shared_ptr<ActorManager> actorManager);
    ~PlayerStateCA3();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //����
    std::shared_ptr<Collidable> m_leftLeg;
    //�U������
    std::shared_ptr<MeleeAttack> m_attackC;
    //�U��������쐬
    void CreateAttack();
    //�U���̏�ԍX�V
    void UpdateAttack();
    //����
    void SpeedDown();
    //�폜
    void DeleteAttack(const std::shared_ptr<AttackManager> attackManager);
};



