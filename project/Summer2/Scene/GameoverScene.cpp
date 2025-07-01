#include "GameoverScene.h"
#include "StageScene.h"
#include "../General/Input.h"
#include "SceneController.h"
#include<DxLib.h>
#include "../General/game.h"
#include "../General/Collision/Physics.h"

namespace {
	constexpr int kAppearInterval = 20;
	constexpr int kFrameMargin = 10;//ゲーム画面からポーズ画面までの幅
}

GameoverScene::GameoverScene(SceneController& controller):
	SceneBase(controller),
	m_update(&GameoverScene::AppearUpdate),
	m_draw(&GameoverScene::NormalDraw),
	m_countFrame(0)
{
}

GameoverScene::~GameoverScene()
{
}

void GameoverScene::Init()
{
	//Physicsを開始
	Physics::GetInstance().StartUpdate();
}

void GameoverScene::Update()
{
	(this->*m_update)();
}

void GameoverScene::Draw()
{
	(this->*m_draw)();
}
void GameoverScene::End()
{
	//Physicsを開始
	Physics::GetInstance().StartUpdate();
}

void GameoverScene::AppearUpdate()
{
	++m_countFrame;
	if (m_countFrame > kAppearInterval) {
		m_countFrame = kAppearInterval;
		m_update = &GameoverScene::NormalUpdate;
		return;
	}
}

void GameoverScene::NormalUpdate()
{
	auto& input = Input::GetInstance();
	//Aボタンで次へ
	if (input.IsTrigger("A")) {
		m_update = &GameoverScene::DisappearUpdate;
		m_countFrame = kAppearInterval;
		return;
	}
}

void GameoverScene::DisappearUpdate()
{
	--m_countFrame;
	if (m_countFrame < 0) {
		m_countFrame = 0;
		//自分の下になってるシーンを切り替える
		m_controller.ChangeBaseScene(std::make_shared<StageScene>(m_controller));
		m_controller.PopScene();//自分は消える
		return;
	}
}

void GameoverScene::NormalDraw()
{
	//背景を赤っぽくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_countFrame);
	DrawBox(0, 0,//左上
		Game::kScreenWidth, Game::kScreenHeight,//右下
		0xff5555,//カラー
		true);//塗り潰す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}