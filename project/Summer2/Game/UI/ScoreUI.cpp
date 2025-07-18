#include "ScoreUI.h"
#include "../../General/game.h"
#include "../Actors/ActorManager.h"
#include "../GameRule/Score.h"
#include <DxLib.h>

namespace
{
	//大きさ
	constexpr float kScale = 0.3f;
	//幅
	constexpr int kImageWidth = 256;
	constexpr int kImageHeight = 256;
	//スコアのX座標
	constexpr int kPosX = (Game::kScreenWidth - 100);
	//スコアのY座標
	constexpr int kPosY = 50;
	//スコアの1桁の幅
	constexpr int kDigitMargin = 50;
	//重力
	constexpr float kGravity = 0.98f;
	//ジャンプ力
	constexpr float kJumpPower = 2.0f;
}

ScoreUI::ScoreUI(int handle, const std::weak_ptr<Score> score) :
	UIBase(UIData::Score,handle),
	m_viewScore(0),
	m_viewMaxScore(0),
	m_score(score),
	m_digits{}
{
	for (auto& pos : m_viewPoses)
	{
		pos = { kPosX ,kPosY };
	}
}

ScoreUI::~ScoreUI()
{
}

void ScoreUI::Init()
{
}

void ScoreUI::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//プレイヤーまたはボスが消えた場合はこのUIも削除
	if (actorManager.lock()->GetPlayer().expired() || actorManager.lock()->GetBoss().expired())
	{
		m_isDelete = true;
		return;
	}
	//スコア更新
	m_viewMaxScore = m_score.lock()->GetScore();
	//現在のスコアがまだ最大まで達していないなら
	if (m_viewScore != m_viewMaxScore)
	{
		auto score = MathSub::Lerp(static_cast<float>(m_viewScore), static_cast<float>(m_viewMaxScore), 0.5f);
		if (score + 1.0f >= static_cast<float>(m_viewMaxScore))
		{
			score = m_viewMaxScore;
		}
		m_viewScore = score;
	}
	//取り出す値
	int score = m_viewScore;
	for (int i = 0;i < kDigitNum;++i)
	{
		//現在の値
		auto digit = m_digits[i];
		//取り出して保存
		m_digits[i] = score % 10;
		//桁を下げる
		score = score / 10;
		//桁が変わってるなら
		if (digit != m_digits[i])
		{
			//少しはねる
			m_viewPoses[i].y -= kJumpPower;
		}
		//これ以上Y座標が下がらないように補正
		m_viewPoses[i].y += kGravity;
		if (m_viewPoses[i].y > kPosY)
		{
			m_viewPoses[i].y = kPosY;
		}
	}
}

void ScoreUI::Draw() const
{
	for (int i = 0;i < kDigitNum;++i)
	{
		//切り取りを計算する
		int sizeX, sizeY;
		GetGraphSize(m_handle, &sizeX, &sizeY);//画像サイズ
		int cutX = m_digits[i] % (sizeX / kImageWidth);//横
		int cutY = m_digits[i] / (sizeX / kImageWidth);//縦
		//描画
		DrawRectRotaGraphFast(m_viewPoses[i].x - i * kDigitMargin, m_viewPoses[i].y,
			kImageWidth * cutX,
			kImageHeight * cutY,
			kImageWidth, kImageHeight,
			kScale, 0.0f, m_handle, true, false);
	}
}

void ScoreUI::End()
{
}
