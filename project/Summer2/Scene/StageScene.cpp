#include "StageScene.h"
#include "../Game/GameManager.h"
#include "ResultScene.h"
#include "DebugScene.h"
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

void StageScene::Update(Input& input)
{
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
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}
	m_gameManager->Update(input);
}

void StageScene::Draw()
{
	m_gameManager->Draw();
}

void StageScene::End()
{
	m_gameManager->End();
}
