#include "GameManager.h"
#include "../General/Input.h"
#include <DxLib.h>
#include "../General/game.h"
#include "Actors/ActorManager.h"
#include "Actors/Character/Player/Player.h"
#include "UI/UIManager.h"
#include "../General/Collision/Physics.h"
#include "../Game/Camera/Camera.h"
#include "../Game/GameRule/Score.h"
#include "../Game/GameRule/Timer.h"
#include <cassert>

namespace
{
	//シャドウマップの描画サイズ
	constexpr int kShadowMapWidth = 1024;
	constexpr int kShadowMapHeight = 1024;
	//ライトの向き
	const VECTOR kLightDir = { 0.0f, -1.0f, 0.5f };
	//シャドウマップの範囲
	constexpr float kShadowMapHorizon = 2000.0f;
	constexpr float kShadowMapVerticalMin = -1.0f;
	constexpr float kShadowMapVerticalMax = 1000.0f;
}

GameManager::GameManager(Stage::StageIndex index):
	m_isGameover(false),
	m_isGameClear(false)
{
	//UIマネージャー
	m_uiManager = std::make_shared<UIManager>();
	//カメラの初期化
	m_camera = std::make_shared<Camera>();
	//アクターマネージャー
	m_actorManager = std::make_shared<ActorManager>(index, m_uiManager);
	//スコア
	m_score = std::make_shared<Score>();
	m_uiManager->CreateScoreUI(m_score);
	//タイマー
	m_timer = std::make_shared<Timer>();
	m_uiManager->CreateTimerUI(m_timer);
	//シャドウマップの準備
	InitShadow();
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	//UIの初期化
	m_uiManager->Init();
	//アクターマネージャーの初期化
	m_actorManager->Init();
	//カメラの初期化
	m_camera->Init();
}

void GameManager::Update()
{
	//デバッグで一時停止されてないなら
	auto& input = Input::GetInstance();
#if _DEBUG
	if (!m_isUpdateStop || (input.IsTrigger("StopUpdate") && m_isUpdateStop))
#endif
	{
		//アクターの更新
		m_actorManager->Update(m_camera,m_score);
		//タイマー
		m_timer->Update();
		//UIの更新
		m_uiManager->Update(m_actorManager);
		//カメラの更新
		m_camera->Update(m_actorManager);
		//ボスを倒したとき
		if (m_actorManager->GetBoss().expired() && !m_isGameClear)
		{
			//プレイヤーの勝利アニメーションが終了したら
			if (m_actorManager->GetPlayer().lock()->IsFinishClearAnim())
			{
				//タイマーをスコアに加算
				m_score->AddTimeScore(m_timer->GetTime());
				//クリア
				m_isGameClear = true;
			}
		}
		//プレイヤーが死亡した際の処理
		else if (m_actorManager->GetPlayer().expired() && !m_isGameover)
		{
			//ゲームオーバー
			m_isGameover = true;
		}
	}
	//シャドウマップに描画する範囲を設定
	//カメラの周囲のみ
	UpdateShadowDrawArea();
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
	//シャドウマップへの描画の準備
	ShadowMap_DrawSetup(m_shadowMapHandle);
	//シャドウマップへのアクターの描画
	m_actorManager->Draw();
	//シャドウマップへの描画を終了
	ShadowMap_DrawEnd();
	//描画に使用するシャドウマップを設定
	SetUseShadowMap(0, m_shadowMapHandle);
	//アクターの描画
	m_actorManager->Draw();
	//UIの描画
	m_uiManager->Draw();
	//描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);
}

void GameManager::End()
{
	//アクターマネージャーの終了
	m_actorManager->End();
	//UIマネージャーの終了
	m_uiManager->End();
	//シャドウマップの削除
	DeleteShadowMap(m_shadowMapHandle);
}

void GameManager::InitShadow()
{
	//ディレクショナルライト
	ChangeLightTypeDir(kLightDir);
	//シャドウマップハンドルの作成
	m_shadowMapHandle = MakeShadowMap(kShadowMapWidth, kShadowMapHeight);
	//シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(m_shadowMapHandle, kLightDir);
}

void GameManager::UpdateShadowDrawArea()
{
	auto shadowMinPos = m_camera->GetPos();
	shadowMinPos.x += -kShadowMapHorizon;
	shadowMinPos.y = kShadowMapVerticalMin;
	shadowMinPos.z += -kShadowMapHorizon;
	auto shadowMaxPos = m_camera->GetPos();
	shadowMaxPos.x += kShadowMapHorizon;
	shadowMaxPos.y = kShadowMapVerticalMax;
	shadowMaxPos.z += kShadowMapHorizon;
	SetShadowMapDrawArea(m_shadowMapHandle, shadowMinPos.ToDxLibVector(), shadowMaxPos.ToDxLibVector());
}
