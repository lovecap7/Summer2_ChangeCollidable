#include "TitleScene.h"
#include "Dxlib.h"
#include  "../General/Input.h"
#include "SceneController.h"
#include "SelectStageScene.h"
#include "../General/Collision/Physics.h"
#include "../Game/UI/UIManager.h"
#include "../Game/Camera/TitleCamera/TitleCamera.h"
#include "../Game/Title/TitlePlayer.h"
#include "../Game/Actors/Stage/StageObjectDraw.h"
#include "../General/CSVDataLoader.h"
#include <memory>
#include <cassert>
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
	//ハンドルロード
	LoadHandle();
	//配置データ
	LoadStage();
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
	//ステージ描画
	for (auto& obj : m_stageObjects)
	{
		obj->Draw();
	}
}

void TitleScene::End()
{
	//プレイヤーの終了
	m_player->End();
	//ハンドル削除
	AllDeleteHandle();
	//オブジェ削除
	AllDeleteStage();
}

void TitleScene::LoadHandle()
{
	m_handles["Block_Grass"] = { MV1LoadModel(L"Data/Model/Stage/1/Block_Grass.mv1") };
	m_handles["Grass1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Grass1.mv1") };
	m_handles["Rock1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Rock1.mv1") };
	m_handles["Rock2"] = { MV1LoadModel(L"Data/Model/Stage/Title/Rock2.mv1") };
	m_handles["Tree1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Tree1.mv1") };

	//ロードに成功したかチェック
	for (auto& [key, value] : m_handles) {
		assert(value >= 0);
	}
}
void TitleScene::LoadStage()
{
	//配置
	auto csvLoader = std::make_unique<CSVDataLoader>();
	auto datas = csvLoader->LoadTransformDataCSV("Data/CSV/TitleTransformData.csv");
	//名前からオブジェクトを配置していく
	for (auto& data : datas)
	{
		if (data.name == "Block_Grass")
		{
			//大きさを1/100しないと大きすぎるので
			data.scale = VScale(data.scale, 0.01f);
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles.at("Block_Grass")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Grass1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles.at("Grass1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Rock1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles.at("Rock1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Rock2")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles.at("Rock2")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Tree1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles.at("Tree1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
	}
}

void TitleScene::AllDeleteHandle()
{
	for (auto& [key, value] : m_handles) {
		if (value >= 0)
		{
			auto result = MV1DeleteModel(value);
			assert(result == 0);
		}
	}
	m_handles.clear();
}

void TitleScene::AllDeleteStage()
{
	m_stageObjects.clear();
	m_player.reset();
	m_camera.reset();
}
