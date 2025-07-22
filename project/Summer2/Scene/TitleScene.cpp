#include "TitleScene.h"
#include <Dxlib.h>
#include  "../General/Input.h"
#include "SceneController.h"
#include "SelectStageScene.h"
#include "../General/Collision/Physics.h"
#include "../Game/UI/UIManager.h"
#include "../Game/Camera/TitleCamera/TitleCamera.h"
#include "../Game/Title/TitlePlayer.h"
#include "../Game/Actors/Stage/StageObjectDraw.h"
#include "../Game/Actors/Stage/Sky.h"
#include "../General/CSVDataLoader.h"
#include <memory>
#include <cassert>
#if _DEBUG
//デバッグモード
#include "DebugScene.h"
#endif

namespace
{
	//シャドウマップの描画サイズ
	constexpr int kShadowMapWidth = 1024 * 2;
	constexpr int kShadowMapHeight = 1024 * 2;
	//ライトの向き
	const VECTOR kLightDir1 = { 0.5f, -1.0f, 0.8f };
	//シャドウマップの範囲
	constexpr float kShadowMapHorizon = 2000.0f;
	constexpr float kShadowMapVerticalMin = -1.0f;
	constexpr float kShadowMapVerticalMax = 1000.0f;
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
	//ライトの初期化
	InitLight();
	//影の初期化
	InitShadow();
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
	//ステージ更新
	for (auto& obj : m_stageObjects)
	{
		obj->Update();
	}
	//影
	UpdateShadow();
}

void TitleScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, L"Title Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
#endif
	//シャドウマップへの描画の準備
	ShadowMap_DrawSetup(m_shadowMapHandle);
	//プレイヤーの描画
	m_player->Draw();
	//ステージ描画
	for (auto& obj : m_stageObjects)
	{
		obj->Draw();
	}
	//シャドウマップへの描画を終了
	ShadowMap_DrawEnd();
	//描画に使用するシャドウマップを設定
	SetUseShadowMap(0, m_shadowMapHandle);
	//プレイヤーの描画
	m_player->Draw();
	//ステージ描画
	for (auto& obj : m_stageObjects)
	{
		obj->Draw();
	}
	//描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);
}

void TitleScene::End()
{
	//プレイヤーの終了
	m_player->End();
	//ハンドル削除
	AllDeleteHandle();
	//オブジェ削除
	AllDeleteStage();
	//ライトの削除
	DeleteLightHandle(m_lightHandle);
	//シャドウマップの削除
	DeleteShadowMap(m_shadowMapHandle);
}

void TitleScene::LoadHandle()
{
	m_stageHandles["Block_Grass"] = { MV1LoadModel(L"Data/Model/Stage/1/Block_Grass.mv1") };
	m_stageHandles["Grass1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Grass1.mv1") };
	m_stageHandles["Rock1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Rock1.mv1") };
	m_stageHandles["Rock2"] = { MV1LoadModel(L"Data/Model/Stage/Title/Rock2.mv1") };
	m_stageHandles["Tree1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Tree1.mv1") };
	m_stageHandles["Flower1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Flower1.mv1") };
	m_stageHandles["Sky"] = { MV1LoadModel(L"Data/Model/Stage/Sky/Sky_Daylight02.pmx") };
	//ロードに成功したかチェック
	for (auto& [key, value] : m_stageHandles) {
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
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Block_Grass")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Grass1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Grass1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Rock1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Rock1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Rock2")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Rock2")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Tree1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Tree1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Flower1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Flower1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
	}
	//空を作成
	m_stageObjects.emplace_back(std::make_shared<Sky>(MV1DuplicateModel(m_stageHandles["Sky"])));
}

void TitleScene::AllDeleteHandle()
{
	for (auto& [key, value] : m_stageHandles) {
		if (value >= 0)
		{
			auto result = MV1DeleteModel(value);
			assert(result == 0);
		}
	}
	m_stageHandles.clear();
}

void TitleScene::AllDeleteStage()
{
	m_stageObjects.clear();
	m_player.reset();
	m_camera.reset();
}

void TitleScene::InitLight()
{
	m_lightHandle = CreateDirLightHandle(kLightDir1);
}

void TitleScene::InitShadow()
{
	//シャドウマップハンドルの作成
	m_shadowMapHandle = MakeShadowMap(kShadowMapWidth, kShadowMapHeight);
	//シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(m_shadowMapHandle, kLightDir1);
}

void TitleScene::UpdateShadow()
{
	auto shadowMinPos = Vector3::Zero();
	shadowMinPos.x = -kShadowMapHorizon;
	shadowMinPos.y = kShadowMapVerticalMin;
	shadowMinPos.z = -kShadowMapHorizon;
	auto shadowMaxPos = Vector3::Zero();
	shadowMaxPos.x = kShadowMapHorizon;
	shadowMaxPos.y = kShadowMapVerticalMax;
	shadowMaxPos.z = kShadowMapHorizon;
	SetShadowMapDrawArea(m_shadowMapHandle, shadowMinPos.ToDxLibVector(), shadowMaxPos.ToDxLibVector());
}
