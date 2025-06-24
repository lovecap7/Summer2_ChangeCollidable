#include "TestCollScene.h"
#include "ResultScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <DxLib.h>
#include <vector>
#include "../General/game.h"
//アクター
#include "../Game/Actors/Actor.h"
#include "../Game/Actors/ActorManager.h"
//プレイヤー
#include "../Game/Actors/Player/Player.h"
//敵
#include "../Game/Actors/Enemy/EnemyManager.h"
#include "../Game/Actors/Enemy/PurpleDinosaur/PurpleDinosaur.h"
#include "../Game/Actors/Enemy/SmallDragon/SmallDragon.h"
#include "../Game/Actors/Enemy/BossDragon/BossDragon.h"
#include "../Game/Actors/Enemy/EnemyBase.h"
//ステージ
#include "../Game/Actors/Stage/InvisibleWall.h"
//カメラ
#include "../Game/Camera/Camera.h"

//デバッグモード
#include "DebugScene.h"

namespace
{
	const Vector3 kPlayerPos = { 0.0f, 0.0f, 0.0f };
	const Vector3 kCameraPos = { 0.0f, 500.0f, -800.0f };
}

TestCollScene::TestCollScene(SceneController& controller) :
	SceneBase(controller),
	m_playerHandle(MV1LoadModel("Data/Model/Player/Player.mv1")),
	m_polygonHandle(MV1LoadModel("Data/Model/Stage/Stage1.mv1")),
	m_wallHandle(MV1LoadModel("Data/Model/Stage/InvisibleWall.mv1")),
	m_purpleDinosaurHandle(MV1LoadModel("Data/Model/Enemy/PurpleDinosaur.mv1")),
	m_smallDragonHandle(MV1LoadModel("Data/Model/Enemy/SmallDragon.mv1")),
	m_bossDragonHandle(MV1LoadModel("Data/Model/Enemy/BossDragon.mv1"))
{
	////登場するオブジェクトをセットしていく
	//std::vector<std::shared_ptr<Actor>> actors;

	////プレイヤーの初期化
	//m_player = std::make_shared<Player>(m_playerHandle, kPlayerPos);
	//actors.push_back(m_player);
	////カメラの初期化
	//m_camera = std::make_unique<Camera>(kCameraPos, m_player);
	//std::shared_ptr<EnemyBase> enemy1 = std::make_shared<PurpleDinosaur>(MV1DuplicateModel(m_purpleDinosaurHandle), Vector3{ -300.0f,-50.0f,0.0f });
	//actors.push_back(enemy1);
	//std::shared_ptr<EnemyBase> enemy2 = std::make_shared<SmallDragon>(MV1DuplicateModel(m_smallDragonHandle), Vector3{ -400.0f,-50.0f,0.0f });
	//actors.push_back(enemy2);
	//std::shared_ptr<EnemyBase> enemy3 = std::make_shared<BossDragon>(MV1DuplicateModel(m_bossDragonHandle), Vector3{ -600.0f,-50.0f,0.0f });
	//actors.push_back(enemy3);
	//actors.push_back(std::make_shared<TestPolygon>(Vector3{ 0.0f,-100.0f,0.0f }, m_polygonHandle));
	//m_actorManager = std::make_shared<ActorManager>(actors, m_player);
}

TestCollScene::~TestCollScene()
{
	//登録解除
	//m_actorManager->Exit();

	MV1DeleteModel(m_playerHandle);
	MV1DeleteModel(m_wallHandle);
	MV1DeleteModel(m_purpleDinosaurHandle);
	MV1DeleteModel(m_polygonHandle);
	MV1DeleteModel(m_smallDragonHandle);
}

void TestCollScene::Init()
{
	//m_actorManager->Init();
}

void TestCollScene::Update(Input& input)
{
#if _DEBUG
	if (input.IsTrigger("StopUpdate"))
	{
		m_isUpdateStop = true;
	}
	if (input.IsTrigger("Enter"))
	{
		m_isUpdateStop = false;
	}
	

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
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}
	if (input.IsTrigger("Start"))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<TestCollScene>(m_controller));
		return;
	}

//	//デバッグで一時停止されてないなら
//#if _DEBUG
//	if (!m_isUpdateStop || (input.IsTrigger("StopUpdate") && m_isUpdateStop))
//#endif
//	{
//		//アクターの更新処理
//		m_actorManager->Update(input,m_camera);
//		//カメラの更新
//		m_camera->Update();
//	}
}

void TestCollScene::Draw()
{
	//DrawString(0, 0, "Stage1 Scene", 0xffffff);
	//DrawString(0, 16, "[D]キーで Debug Scene", 0xffffff);
	//DrawString(0, 40, "Startボタンで リセット", 0xffffff);
	//for (int z = -500; z <= 500; z += 100)
	//{
	//	DrawLine3D(VGet(-500, 0, z), VGet(500, 0, z), 0xff0000);
	//}
	//for (int x = -500; x <= 500; x += 100)
	//{
	//	DrawLine3D(VGet(x, 0, -500), VGet(x, 0, 500), 0x0000ff);
	//}
	//VECTOR screenPos = ConvWorldPosToScreenPos(VGet(500, 0, 0));
	//DrawString(screenPos.x, screenPos.y, "X+", 0xffffff);
	//screenPos = ConvWorldPosToScreenPos(VGet(-500, 0, 0));
	//DrawString(screenPos.x, screenPos.y, "X-", 0xffffff);
	//screenPos = ConvWorldPosToScreenPos(VGet(0, 0, 500));
	//DrawString(screenPos.x, screenPos.y, "Z+", 0xffffff);
	//screenPos = ConvWorldPosToScreenPos(VGet(0, 0, -500));
	//DrawString(screenPos.x, screenPos.y, "Z-", 0xffffff);
	////アクターの描画
	//m_actorManager->Draw();
}

void TestCollScene::End()
{
}
