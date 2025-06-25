#include "PlayerStateBase.h"
class Player;
class Camera;
class PlayerStateCA1 :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateCA1>
{
public:
    PlayerStateCA1(std::weak_ptr<Player> player);
    ~PlayerStateCA1();
    void Init() override;
    void Update(const std::weak_ptr<Camera> camera) override;
private:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
private:
    //�������Ă���
    void SpeedDown();
};



