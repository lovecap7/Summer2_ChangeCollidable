#include "GameManager.h"
#include "../General/Input.h"
#include <DxLib.h>
#include <vector>
#include "../General/game.h"
#include "StageSetup.h"
#include "Actors/ActorManager.h"
#include "Actors/Player/Player.h"
//UI
#include "../Game/UI/UIManager.h"
#include "../Game/UI/UIPlayerHP.h"
#include "../Game/UI/UIPlayerUltGage.h"
//カメラ
#include "../Game/Camera/Camera.h"
#include <cassert>

namespace
{
	const Vector3 kCameraPos = { 0.0f, 600.0f, -600.0f };
}

GameManager::GameManager()
{
	//ステージを作成
	m_stageSetup = std::make_shared<StageSetup>();
	m_stageSetup->MovePlayerPointer(m_player);
	//アクターマネージャー
	m_actorManager = std::make_shared<ActorManager>(m_player);
	//カメラの初期化
	m_camera = std::make_unique<Camera>(kCameraPos, m_player);
	//UIの準備
	m_uiManager = std::make_shared<UIManager>();
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	//セットアップクラスからアクターを受け取る
	//初期化処理
	m_actorManager->Init(m_stageSetup);
	//UIの作成
	//プレイヤーの体力と必殺ゲージ
	auto playerHpUI = std::make_shared<UIPlayerHP>(m_player->GetHurtPoint());
	auto playerUltUI = std::make_shared<UIPlayerUltGage>(m_player->GetUltGage());
	m_uiManager->Entry(playerHpUI);
	m_uiManager->Entry(playerUltUI);
}

void GameManager::Update(Input& input)
{
	//デバッグで一時停止されてないなら
#if _DEBUG
	if (!m_isUpdateStop || (input.IsTrigger("StopUpdate") && m_isUpdateStop))
#endif
	{
		//アクターの更新処理
		m_actorManager->Update(input, m_camera, m_uiManager);
		//カメラの更新
		m_camera->Update();
		//UIの更新
		m_uiManager->Update();
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
	//終了処理
	m_stageSetup->End();
	//登録解除
	m_actorManager->End();
}
