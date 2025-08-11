#include "SelectStageScene.h"
#include "TitleScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include "../General/Fader.h"
#include "../General/CSVDataLoader.h"
#include "../General/StageFigure.h"
#include "../General/Sound/SoundManager.h"
#include "../Game/Actors/Character/Player/SelectStagePlayer.h"
#include "../Game/Camera/SelectStageCamera/SelectStageCamera.h"
#include "../Game/UI/Select/SelectStageBackUI.h"
#include "../SaveData/SaveDataManager.h"
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
	m_stageIndex(static_cast<int>(Stage::StageIndex::Stage1)),
	m_unlockStageIndex(Stage::StageIndex::Stage1),
	m_isDecide(false)
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
	//解放しているステージまでしか選べなくするための処理
	NowUnlockStage();

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

	//背景(生成する順番は描画下から)
	auto back1 = std::make_shared<SelectStageBackUI>(LoadGraph(L"Data/UI/Back/TestImage1.png"));
	auto back2 = std::make_shared<SelectStageBackUI>(LoadGraph(L"Data/UI/Back/TestImage2.png"));
	auto back3 = std::make_shared<SelectStageBackUI>(LoadGraph(L"Data/UI/Back/TestImage3.png"));
	back3->Init();
	back2->Init();
	back1->Init();
	m_stage1Back = back1;
	m_stage2Back = back2;
	//BGM
	SoundManager::GetInstance().PlayBGM("SelectStageBGM");
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
	//真っ暗になったら
	if (fader.IsFinishFadeOut())
	{
		//決定してるなら
		if (m_isDecide)
		{
			//次のシーンへ
			m_controller.ChangeScene(std::make_shared<StageScene>(m_controller, static_cast<Stage::StageIndex>(m_stageIndex)));
			return;
		}
		else
		{
			//タイトルへ
			m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
			return;
		}
	}
	if (!fader.IsFadeNow())
	{
		//決定ボタンをおしたら
		if (input.IsTrigger("A"))
		{
			m_isDecide = true;
			//だんだん暗く
			fader.FadeOut(kFadeSpeed);
		}
		//戻るボタンをおしたら
		else if (input.IsTrigger("B"))
		{
			//だんだん暗く
			fader.FadeOut(kFadeSpeed);
		}
	}
	//ステージを選ぶ
	SelectStageIndex(input);
	//ステージに合わせて背景を動かす
	ChangeBack();
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
#endif
	//プレイヤーの描画
	m_player->Draw();
	//ステージのフィギュアの描画
	for (auto& stageFigure : m_stageFigures)
	{
		stageFigure->Draw();
	}	

	DrawFormatString(0, 80, 0xffff00,L"StageIndex : %d",m_stageIndex);
	DrawFormatString(0, 80, 0xffff00,L"UnlockStageIndex : %d",static_cast<int>(m_unlockStageIndex));
	
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
	//現状のデータをセーブ
	SaveDataManager::GetInstance().Save();
}

void SelectStageScene::Restart()
{
}

void SelectStageScene::SelectStageIndex(Input& input)
{
	//解放されてるステージまでしか選べない
	if (input.IsTrigger("Left"))--m_stageIndex;
	if (input.IsTrigger("Right"))++m_stageIndex;
	m_stageIndex = MathSub::ClampInt(m_stageIndex, static_cast<int>(Stage::StageIndex::Stage1), static_cast<int>(m_unlockStageIndex));
}

void SelectStageScene::ChangeBack()
{
	if (m_stageIndex == static_cast<int>(Stage::StageIndex::Stage1))
	{
		m_stage1Back.lock()->SetAppear(true);
		m_stage2Back.lock()->SetAppear(true);
	}
	else if (m_stageIndex == static_cast<int>(Stage::StageIndex::Stage2))
	{
		m_stage1Back.lock()->SetAppear(false);
		m_stage2Back.lock()->SetAppear(true);
	}
	else
	{
		m_stage1Back.lock()->SetAppear(false);
		m_stage2Back.lock()->SetAppear(false);
	}
}

void SelectStageScene::NowUnlockStage()
{
	auto& saveDatas = SaveDataManager::GetInstance();
	//ステージ1をクリアしていないなら
	if (!saveDatas.IsClearStage(Stage::StageIndex::Stage1))
	{
		//ステージ1しか選べなくする
		m_unlockStageIndex = Stage::StageIndex::Stage1;
	}
	//ステージ2をクリアしていないなら
	else if (!saveDatas.IsClearStage(Stage::StageIndex::Stage2))
	{
		//ステージ2までしか選べなくする
		m_unlockStageIndex = Stage::StageIndex::Stage2;
	}
	//ここまで来たら全て解放
	else if (!saveDatas.IsClearStage(Stage::StageIndex::Stage3))
	{
		//全ステージ解放
		m_unlockStageIndex = Stage::StageIndex::Stage3;
	}
}