#include "GameoverScene.h"
#include "StageScene.h"
#include "../General/Input.h"
#include "SceneController.h"
#include<DxLib.h>
#include "../General/game.h"
#include "../General/Collision/Physics.h"
#include "../General/Fader.h"

namespace {
	constexpr int kAppearInterval = 20;
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
	//Physicsを止める
	Physics::GetInstance().StopUpdate();
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

void GameoverScene::Restart()
{
}

void GameoverScene::AppearUpdate()
{
	++m_countFrame;
	if (m_countFrame > kAppearInterval)
	{
		m_countFrame = kAppearInterval;
		m_update = &GameoverScene::NormalUpdate;
		return;
	}
}

void GameoverScene::NormalUpdate()
{
	auto& input = Input::GetInstance();
	//Aボタンで次へ
	if (input.IsTrigger("A")) 
	{
		//だんだん暗く
		auto& fader = Fader::GetInstance();
		fader.FadeOut();
		m_update = &GameoverScene::DisappearUpdate;
		return;
	}
}

void GameoverScene::DisappearUpdate()
{
	auto& fader = Fader::GetInstance();
	//暗くなったら
	if (fader.IsFinishFadeOut())
	{
		//自分の下になってるシーンを初期化
		m_controller.RestartBaseScene();
		m_controller.PopScene();//自分は消える
		return;
	}
}

void GameoverScene::NormalDraw()
{
	//背景を赤っぽくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 10 * m_countFrame);
	DrawBox(0, 0,//左上
		Game::kScreenWidth, Game::kScreenHeight,//右下
		0xff5555,//カラー
		true);//塗り潰す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}