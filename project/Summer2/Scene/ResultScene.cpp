#include <DxLib.h>
#include <memory>
#include "ResultScene.h"
#include "TitleScene.h"
#include  "../General/Input.h"
#include "SceneController.h"

#if _DEBUG
//デバッグモード
#include "DebugScene.h"
#endif

namespace
{

}

ResultScene::ResultScene(SceneController& controller):
	SceneBase(controller)
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Init()
{
	//なし
}

void ResultScene::Update(Input& input)
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
		m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
		return;
	}
}

void ResultScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Result Scene", 0xffffff);
	DrawString(0, 16, "[D]キーで Debug Scene", 0xffffff);
#endif
}

void ResultScene::End()
{
}
