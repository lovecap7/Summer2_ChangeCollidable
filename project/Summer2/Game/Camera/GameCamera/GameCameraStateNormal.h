#pragma once
#include "GameCameraStateBase.h"
class GameCameraStateNormal :
    public GameCameraStateBase, public std::enable_shared_from_this<GameCameraStateNormal>
{
public:
	GameCameraStateNormal(std::weak_ptr<GameCamera> camera);
	virtual ~GameCameraStateNormal() {};
	//初期化処理
	void Init()override;
	//状態に応じた更新処理
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
private:
	//移動フレーム
	int m_moveFrame;
};

