#include "PauseScene.h"
#include "../General/Input.h"
#include "SceneController.h"
#include<DxLib.h>
#include "../General/game.h"
#include "../General/Collision/Physics.h"

namespace {
	constexpr int kAppearInterval = 20;
	constexpr int kFrameMargin = 10;//ゲーム画面からポーズ画面までの幅
}


PauseScene::PauseScene(SceneController& controller):
	SceneBase(controller),
	m_update(&PauseScene::AppearUpdate),
	m_draw(&PauseScene::ShiftingDraw),
	m_countFrame(0)
{
}

PauseScene::~PauseScene()
{
}

void PauseScene::Init()
{
	//Physicsを止める
	Physics::GetInstance().StopUpdate();
}

void PauseScene::Update()
{
	(this->*m_update)();
}

void PauseScene::Draw()
{
	(this->*m_draw)();
}

void PauseScene::End()
{
	//Physicsを開始
	Physics::GetInstance().StartUpdate();
}

void PauseScene::Restart()
{
}

void PauseScene::AppearUpdate()
{
	++m_countFrame;
	if (m_countFrame > kAppearInterval) 
	{
		m_update = &PauseScene::NormalUpdate;
		m_draw = &PauseScene::NormalDraw;
		return;
	}
}

void PauseScene::NormalUpdate()
{
	auto& input = Input::GetInstance();
	//Pボタンでポーズ解除
	if (input.IsTrigger("Pause")) 
	{
		m_update = &PauseScene::DisappearUpdate;
		m_draw = &PauseScene::ShiftingDraw;
		m_countFrame = kAppearInterval;
		return;
	}
}

void PauseScene::DisappearUpdate()
{
	--m_countFrame;
	if (m_countFrame < 0) 
	{
		m_controller.PopScene();
		return;
	}
}

void PauseScene::NormalDraw()
{
	float wSize = Game::kScreenWidth;
	float hSize = Game::kScreenHeight;
	//背景を白っぽくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(10, 10,//左上
		wSize - 10, hSize - 10,//右下
		0xffffff,//カラー
		true);//塗り潰す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ポーズ画面枠
	DrawBoxAA(10.0f, 10.0f,//左上
		wSize - 10.0f, hSize - 10.0f,//右下
		0xffffff,//カラー
		false,//塗り潰さない
		3.0f);//ちょっと太目の線

	DrawString(30, 30, " Pause ", 0xffffff);
	DrawString(wSize / 3.0f, hSize / 2.0f, " 今後メニューを実装予定です ", 0xff0000);
}

void PauseScene::ShiftingDraw()
{
	float wSize = Game::kScreenWidth;
	float hSize = Game::kScreenHeight;

	//元の高さを計算する
	const int frameHeight = hSize - 10 - 10;
	//真ん中のY座標を計算する
	const int middleY = (10 + hSize - 10) / 2;//真ん中のY座標

	//割合計算
	float rate = static_cast<float>(m_countFrame) / static_cast<float>(kAppearInterval);

	//現在の時間であり得るべき高さの半分を計算します
	int halfHeight = static_cast<int>((frameHeight * rate) * 0.5f);

	//ここから、top(上)とbottom(下)を計算します
	int top = middleY - halfHeight;
	int bottom = middleY + halfHeight;
	//背景を白っぽくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
	DrawBox(10, top,//左上
		wSize - 10, bottom,//右下
		0xffffff,//カラー
		true);//塗り潰す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ポーズ画面枠
	DrawBoxAA(10.0f, static_cast<float>(top),//左上
		wSize - 10.0f, static_cast<float>(bottom),//右下
		0xffffff,//カラー
		false,//塗り潰さない
		3.0f);//ちょっと太目の線
}
