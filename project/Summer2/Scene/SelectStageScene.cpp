#include "SelectStageScene.h"
#include "StageScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <memory>
#include <DxLib.h>
#if _DEBUG
//デバッグモード
#include "DebugScene.h"
#endif

SelectStageScene::SelectStageScene(SceneController& controller):
	SceneBase(controller)
{
	//なし
}

SelectStageScene::~SelectStageScene()
{
}

void SelectStageScene::Init()
{
	//なし
}

void SelectStageScene::Update(Input& input)
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
	if (input.IsTrigger("A"))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<StageScene>(m_controller));
		return;
	}
	if (input.IsTrigger("B"))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<StageScene>(m_controller));
		return;
	}
	if (input.IsTrigger("X"))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<StageScene>(m_controller));
		return;
	}
}

void SelectStageScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "SelectStage Scene", 0xffffff);
	DrawString(0, 16, "[D]キーで Debug Scene", 0xffffff);
	DrawString(0, 32, "A Bottun : Stage 1", 0xffff00);
	DrawString(0, 48, "B Bottun : Stage 2", 0xffff00);
	DrawString(0, 64, "X Bottun : Stage 3", 0xffff00);
#endif
}

void SelectStageScene::End()
{
}
