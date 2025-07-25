#include "SelectStageScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include "../General/Fader.h"
#include "../General/CSVDataLoader.h"
#include "../General/StageFigure.h"
#include "../Game/Actors/Character/Player/SelectStagePlayer.h"
#include "../Game/Camera/SelectStageCamera/SelectStageCamera.h"
#include "StageScene.h"
#include <memory>
#include <DxLib.h>
#if _DEBUG
//デバッグモード
#include "DebugScene.h"
#endif

namespace
{
	constexpr float kFadeSpeed = 2.0f; // フェード速度
}

SelectStageScene::SelectStageScene(SceneController& controller):
	SceneBase(controller),
	m_stageIndex(static_cast<int>(Stage::StageIndex::Stage1))
{
	//CSVデータローダー
	auto csvLodader = std::make_shared<CSVDataLoader>();
	//CSVから座標データを読み込む
	auto tData = csvLodader->LoadTransformDataCSV("Data/CSV/SelectStageTransformData.csv");
	//ステージの位置
	for (auto& data : tData)
	{
		if (data.name == "1")
		{
			m_stagePos[Stage::StageIndex::Stage1] = data.pos;
		}
		else if (data.name == "2")
		{
			m_stagePos[Stage::StageIndex::Stage2] = data.pos;
		}
		else if (data.name == "3")
		{
			m_stagePos[Stage::StageIndex::Stage3] = data.pos;
		}
	}
	//カメラ
	m_camera = std::make_unique<SelectStageCamera>(m_stagePos[static_cast<Stage::StageIndex>(m_stageIndex)]);
	//プレイヤー
	m_player = std::make_unique<SelectStagePlayer>(m_camera->GetPos(),m_stagePos[static_cast<Stage::StageIndex>(m_stageIndex)]);
	//ステージのフィギュア
	for (int i = 0; i < m_stageFigures.size(); ++i)
	{
		//ステージのフィギュアを作成
		m_stageFigures[i] = std::make_shared<StageFigure>();
	}
}

SelectStageScene::~SelectStageScene()
{
}

void SelectStageScene::Init()
{
	auto& fader = Fader::GetInstance();
	//だんだん明るく
	fader.FadeIn(kFadeSpeed);
	//カメラの初期化
	m_camera->Init();
	//プレイヤーの初期化
	m_player->Init();
	//ステージのフィギュアの初期化
	m_stageFigures[0]->Init(MV1LoadModel(L"Data/Model/Stage/Select/Stage1Model.mv1"),m_stagePos[Stage::StageIndex::Stage1]);
	m_stageFigures[1]->Init(MV1LoadModel(L"Data/Model/Stage/Select/Stage1Model.mv1"),m_stagePos[Stage::StageIndex::Stage2]);
	m_stageFigures[2]->Init(MV1LoadModel(L"Data/Model/Stage/Select/Stage1Model.mv1"),m_stagePos[Stage::StageIndex::Stage3]);
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
	if (input.IsTrigger("A"))
	{
		//だんだん暗く
		fader.FadeOut(kFadeSpeed);
	}
	//真っ暗になったら
	if (fader.IsFinishFadeOut())
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<StageScene>(m_controller));
		return;
	}
	//ステージを選ぶ
	SelectStageIndex(input);
	//カメラの更新
	m_camera->Update(m_stagePos[static_cast<Stage::StageIndex>(m_stageIndex)]);
	//プレイヤーの更新
	m_player->Update(m_camera->GetPos(), m_stagePos[static_cast<Stage::StageIndex>(m_stageIndex)]);
	//ステージのフィギュアの更新
	for (auto& stageFigure : m_stageFigures)
	{
		stageFigure->Update();
	}
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
	//ステージのフィギュアの描画
	for (auto& stageFigure : m_stageFigures)
	{
		stageFigure->Draw();
	}	

	DrawFormatString(0, 80, 0xffff00,L"StageIndex : %d",m_stageIndex);
}

void SelectStageScene::End()
{
	//プレイヤーの終了
	m_player->End();
	//ステージのフィギュアの終了
	for (auto& stageFigure : m_stageFigures)
	{
		stageFigure->End();
	}
}

void SelectStageScene::Restart()
{
}

void SelectStageScene::SelectStageIndex(Input& input)
{
	if (input.IsTrigger("Left"))--m_stageIndex;
	if (input.IsTrigger("Right"))++m_stageIndex;
	m_stageIndex = MathSub::ClampInt(m_stageIndex, static_cast<int>(Stage::StageIndex::Stage1), static_cast<int>(Stage::StageIndex::Stage3));
}