#include "StageScene.h"
#include "../Game/GameManager.h"
#include "ResultScene.h"
#include "DebugScene.h"
#include "PauseScene.h"
#include "GameoverScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <DxLib.h>
#include <vector>
#include "../General/game.h"

StageScene::StageScene(SceneController& controller):
	SceneBase(controller)
{
	m_gameManager = std::make_unique<GameManager>();
}

StageScene::~StageScene()
{
}

void StageScene::Init()
{
	m_gameManager->Init();
}

void StageScene::Update()
{
	auto& input = Input::GetInstance();
#if _DEBUG
	//デバッグシーン
	if (input.IsTrigger("SceneChange"))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}
#endif
	if (input.IsTrigger("Pause"))
	{
		//ポーズ
		m_controller.PushScene(std::make_shared<PauseScene>(m_controller));
		return;
	}
	//ゲームの更新
	m_gameManager->Update();
	//ゲームオーバーしたときの処理
	if (m_gameManager->IsGameover())
	{
		m_controller.PushScene(std::make_shared<GameoverScene>(m_controller));
		return;
	}
}

void StageScene::Draw()
{
	m_gameManager->Draw();
}

void StageScene::End()
{
	m_gameManager->End();
}
