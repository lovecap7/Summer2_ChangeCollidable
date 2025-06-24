#include "DebugScene.h"
#include "TitleScene.h"
#include "SelectStageScene.h"
#include "StageScene.h"
#include "ResultScene.h"
#include "TestCollScene.h"
#include <DxLib.h>
#include  "../General/Input.h"
#include "SceneController.h"
#include <memory>

namespace
{
	constexpr int kSceneNum = 7;
}

DebugScene::DebugScene(SceneController& controller):
	SceneBase(controller),
	m_selectSceneIndex(0)
{
}

DebugScene::~DebugScene()
{
}

void DebugScene::Init()
{
	//なし
}

void DebugScene::Update(Input& input)
{
#if _DEBUG
	if (input.IsTrigger("Up")) --m_selectSceneIndex;
	if (input.IsTrigger("Down")) ++m_selectSceneIndex;

	if (m_selectSceneIndex < 0)
	{
		m_selectSceneIndex = kSceneNum - 1;
	}
	else if (m_selectSceneIndex >= kSceneNum)
	{
		m_selectSceneIndex = 0;
	}
	if (input.IsTrigger("Ok"))
	{
		switch (m_selectSceneIndex)
		{
		case 0:
			//次のシーンへ
			m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
			return;
			break;
		case 1:
			//次のシーンへ
			m_controller.ChangeScene(std::make_shared<SelectStageScene>(m_controller));
			return;
			break;
		case 2:
			//次のシーンへ
			m_controller.ChangeScene(std::make_shared<StageScene>(m_controller));
			return;
			break;
		case 3:
			//次のシーンへ
			m_controller.ChangeScene(std::make_shared<StageScene>(m_controller));
			return;
			break;
		case 4:
			//次のシーンへ
			m_controller.ChangeScene(std::make_shared<StageScene>(m_controller));
			return;
			break;
		case 5:
			//次のシーンへ
			m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
			return;
			break;
		case 6:
			//次のシーンへ
			m_controller.ChangeScene(std::make_shared<TestCollScene>(m_controller));
			return;
			break;
		default:
			break;
		}
	}
	
#endif
}

void DebugScene::Draw()
{
	DrawString(0, 0, "Debug Scene", 0xff0000);
	DrawString(100, 50, "Title Scene", 0xffffff);
	DrawString(100, 70, "SelectStage Scene", 0xffffff);
	DrawString(100, 90, "Stage1 Scene", 0xffffff);
	DrawString(100, 110, "Staeg2 Scene", 0xffffff);
	DrawString(100, 130, "Stage3 Scene", 0xffffff);
	DrawString(100, 150, "Result Scene", 0xffffff);
	DrawString(100, 180, "TestCollt Scene", 0xffffff);
	switch (m_selectSceneIndex)
	{
	case 0:
		DrawString(100, 50, "Title Scene", 0xff0000);
		break;
	case 1:
		DrawString(100, 70, "SelectStage Scene", 0xff0000);
		break;
	case 2:
		DrawString(100, 90, "Stage1 Scene", 0xff0000);
		break;
	case 3:
		DrawString(100, 110, "Staeg2 Scene", 0xff0000);
		break;
	case 4:
		DrawString(100, 130, "Stage3 Scene", 0xff0000);
		break;
	case 5:
		DrawString(100, 150, "Result Scene", 0xff0000);
		break;
	case 6:
		DrawString(100, 180, "TestCollt Scene", 0xff0000);
		break;
	default:
		break;
	}
}

void DebugScene::End()
{
}
