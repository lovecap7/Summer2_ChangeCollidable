#include "GameManager.h"
#include "../General/Input.h"
#include <DxLib.h>
#include <vector>
#include "../General/game.h"
#include "StageSetup.h"
#include "Actors/Player/Player.h"
#include "../General/Collision/Physics.h"

#include "../Game/Camera/Camera.h"
#include <cassert>

namespace
{
	const Vector3 kCameraPos = { 0.0f, 600.0f, -600.0f };
}

GameManager::GameManager()
{
	//ステージを作成
	m_stageSetup = std::make_shared<StageSetup>();
	//プレイヤーを受け取る
	m_stageSetup->MovePlayerPointer(m_player);
	//アクターを受け取る
	m_stageSetup->MoveActorsPointer(m_actors);
	//カメラの初期化
	m_camera = std::make_shared<Camera>(kCameraPos, m_player);
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	//アクターの初期化処理
	for (auto& actor : m_actors)
	{
		actor->Init();
	}
}

void GameManager::Update(Input& input)
{
	//デバッグで一時停止されてないなら
#if _DEBUG
	if (!m_isUpdateStop || (input.IsTrigger("StopUpdate") && m_isUpdateStop))
#endif
	{
		//アクターの更新
		for (auto& actor : m_actors)
		{
			actor->Update(m_camera);
		}
		//当たり判定


		//消滅フラグチェック
		auto remIt = std::remove_if(m_actors.begin(), m_actors.end(), [](std::shared_ptr<Actor> actor) {
			bool isDead = actor->IsDelete();
			return isDead;
			});
		m_actors.erase(remIt, m_actors.end());//削除

		//カメラの更新
		m_camera->Update();
		
	}
}

void GameManager::Draw() const
{
#if _DEBUG
	DrawString(0, 0, "Stage1 Scene", 0xffffff);
	DrawString(0, 16, "[D]キーで Debug Scene", 0xffffff);

	for (int z = -500; z <= 500; z += 100)
	{
		DrawLine3D(VGet(-500, 0, z), VGet(500, 0, z), 0xff0000);
	}
	for (int x = -500; x <= 500; x += 100)
	{
		DrawLine3D(VGet(x, 0, -500), VGet(x, 0, 500), 0x0000ff);
	}
	VECTOR screenPos = ConvWorldPosToScreenPos(VGet(500, 0, 0));
	DrawString(screenPos.x, screenPos.y, "X+", 0xffffff);
	screenPos = ConvWorldPosToScreenPos(VGet(-500, 0, 0));
	DrawString(screenPos.x, screenPos.y, "X-", 0xffffff);
	screenPos = ConvWorldPosToScreenPos(VGet(0, 0, 500));
	DrawString(screenPos.x, screenPos.y, "Z+", 0xffffff);
	screenPos = ConvWorldPosToScreenPos(VGet(0, 0, -500));
	DrawString(screenPos.x, screenPos.y, "Z-", 0xffffff);
#endif
	//アクターの描画
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}
	
}

void GameManager::End()
{
	//アクターの終了処理
	for (auto& actor : m_actors)
	{
		actor->End();
	}

	m_stageSetup->End();
}
