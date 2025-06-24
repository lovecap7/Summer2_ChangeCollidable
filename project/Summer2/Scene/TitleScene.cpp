#include "TitleScene.h"
#include "Dxlib.h"
#include  "../General/Input.h"
#include "SceneController.h"
#include "SelectStageScene.h"
#include <memory>
#if _DEBUG
//デバッグモード
#include "DebugScene.h"
#endif

namespace
{
}

TitleScene::TitleScene(SceneController& controller):
	SceneBase(controller)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	//なし
}

void TitleScene::Update(Input& input)
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
		m_controller.ChangeScene(std::make_shared<SelectStageScene>(m_controller));
		return;
	}
}

void TitleScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Title Scene", 0xffffff);
	DrawString(0, 16, "[D]キーで Debug Scene", 0xffffff);
#endif
}

void TitleScene::End()
{
}
