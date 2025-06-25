#include "PlayerStateBase.h"
class Player;
class Camera;
class PlayerStateCA2 :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateCA2>
{
public:
    PlayerStateCA2(std::weak_ptr<Player> player);
    ~PlayerStateCA2();
    void Init() override;
    void Update(const std::weak_ptr<Camera> camera) override;
private:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
private:
    //�������Ă���
    void SpeedDown();
};



