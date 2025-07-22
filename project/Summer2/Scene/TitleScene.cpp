#include "TitleScene.h"
#include "Dxlib.h"
#include  "../General/Input.h"
#include "SceneController.h"
#include "SelectStageScene.h"
#include "../General/Collision/Physics.h"
#include "../Game/UI/UIManager.h"
#include "../Game/Camera/TitleCamera/TitleCamera.h"
#include "../Game/Title/TitlePlayer.h"

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
	//カメラ
	m_camera = std::make_unique<TitleCamera>();
	//プレイヤー
	m_player = std::make_unique<TitlePlayer>();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	//カメラの初期化
	m_camera->Init();
	//プレイヤー
	m_player->Init();
}

void TitleScene::Update()
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
	if (input.IsTrigger("B"))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<SelectStageScene>(m_controller));
		return;
	}
	//カメラ更新
	m_camera->Update();
	//プレイヤー更新
	m_player->Update();
}

void TitleScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, L"Title Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
#endif
	//プレイヤーの描画
	m_player->Draw();
}

void TitleScene::End()
{
	//プレイヤーの終了
	m_player->End();
}