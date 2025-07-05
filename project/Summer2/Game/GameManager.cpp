#include "GameManager.h"
#include "../General/Input.h"
#include <DxLib.h>
#include "../General/game.h"
#include "Actors/ActorManager.h"
#include "Actors/Character/Player/Player.h"
#include "UI/UIManager.h"
#include "../General/Collision/Physics.h"
#include "../Game/Camera/Camera.h"
#include "../Game/GameRule/Score.h"
#include <cassert>

GameManager::GameManager(Stage::StageIndex index)
{
	//UIマネージャー
	m_uiManager = std::make_shared<UIManager>();
	//カメラの初期化
	m_camera = std::make_shared<Camera>();
	//アクターマネージャー
	m_actorManager = std::make_shared<ActorManager>(index, m_uiManager);
	//スコア
	m_score = std::make_shared<Score>();
	m_uiManager->CreateScoreUI(m_score);
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	//UIの初期化
	m_uiManager->Init();
	//アクターマネージャーの初期化
	m_actorManager->Init();
	//カメラの初期化
	m_camera->Init();
}

void GameManager::Update()
{
	//デバッグで一時停止されてないなら
	auto& input = Input::GetInstance();
#if _DEBUG
	if (!m_isUpdateStop || (input.IsTrigger("StopUpdate") && m_isUpdateStop))
#endif
	{
		//アクターの更新
		m_actorManager->Update(m_camera,m_score);
		//UIの更新
		m_uiManager->Update(m_actorManager);
		//カメラの更新
		m_camera->Update(m_actorManager);
		//ボスを倒したとき
		if (m_actorManager->GetBoss().expired())
		{
			//プレイヤーの勝利アニメーションが終了したら
			if (m_actorManager->GetPlayer().lock()->IsFinishClearAnim())
			{
				m_isGameClear = true;
			}
		}
		//プレイヤーが死亡した際の処理
		else if (m_actorManager->GetPlayer().expired())
		{
			m_isGameover = true;
		}
	}
}

void GameManager::Draw() const
{
#if _DEBUG
	DrawString(0, 0, "Stage1 Scene", 0xffffff);
	DrawString(0, 16, "[D]キーで Debug Scene", 0xffffff);

	for (int z = -500; z <= 500; z += 100)
	{
		DrawLine3D(VGet(-500, 0, z), VGet(500, 0, z), 0xff0000);
	}
	for (int x = -500; x <= 500; x += 100)
	{
		DrawLine3D(VGet(x, 0, -500), VGet(x, 0, 500), 0x0000ff);
	}
	VECTOR screenPos = ConvWorldPosToScreenPos(VGet(500, 0, 0));
	DrawString(screenPos.x, screenPos.y, "X+", 0xffffff);
	screenPos = ConvWorldPosToScreenPos(VGet(-500, 0, 0));
	DrawString(screenPos.x, screenPos.y, "X-", 0xffffff);
	screenPos = ConvWorldPosToScreenPos(VGet(0, 0, 500));
	DrawString(screenPos.x, screenPos.y, "Z+", 0xffffff);
	screenPos = ConvWorldPosToScreenPos(VGet(0, 0, -500));
	DrawString(screenPos.x, screenPos.y, "Z-", 0xffffff);
#endif
	//アクターの描画
	m_actorManager->Draw();
	//UIの描画
	m_uiManager->Draw();
}

void GameManager::End()
{
	//アクターマネージャーの終了
	m_actorManager->End();
	//UIマネージャーの終了
	m_uiManager->End();
}
