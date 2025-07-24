#include "SelectStageScene.h"
#include "StageScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include "../General/Fader.h"
#include "../Game/Actors/Character/Player/SelectStagePlayer.h"
#include "../Game/Camera/SelectStageCamera/SelectStageCamera.h"
#include <memory>
#include <DxLib.h>
#if _DEBUG
//デバッグモード
#include "DebugScene.h"
#endif

SelectStageScene::SelectStageScene(SceneController& controller):
	SceneBase(controller)
{
	//カメラ
	m_camera = std::make_unique<SelectStageCamera>();
	//プレイヤー
	m_player = std::make_unique<SelectStagePlayer>();
}

SelectStageScene::~SelectStageScene()
{
}

void SelectStageScene::Init()
{
	auto& fader = Fader::GetInstance();
	//だんだん明るく
	fader.FadeIn();
	//カメラの初期化
	m_camera->Init();
	//プレイヤーの初期化
	m_player->Init();
}

void SelectStageScene::Update()
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
	auto& fader = Fader::GetInstance();
	//何かボタンをおしたら
	if (input.IsTriggerAny())
	{
		//だんだん暗く
		fader.FadeOut();
	}
	//真っ暗になったら
	if (fader.IsFinishFadeOut())
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<StageScene>(m_controller));
		return;
	}
	//カメラの更新
	m_camera->Update();
	//プレイヤーの更新
	m_player->Update();
}

void SelectStageScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, L"SelectStage Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
	DrawString(0, 32, L"A Bottun : Stage 1", 0xffff00);
#endif
	DrawString(0, 48, L"このシーン(ステージセレクト)はまだ未実装です。", 0xffff00);
	DrawString(0, 64, L"何かボタンを押して次のシーンに移動してください", 0xffff00);
	//プレイヤーの描画
	m_player->Draw();
}

void SelectStageScene::End()
{
	//プレイヤーの終了
	m_player->End();
}

void SelectStageScene::Restart()
{
}
