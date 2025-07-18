#include "ScoreUI.h"
#include "../../General/Math/MathSub.h"
#include "../../General/game.h"
#include "../Actors/ActorManager.h"
#include "../GameRule/Score.h"
#include <DxLib.h>

namespace
{
	constexpr int kImageWidth = 256;
	constexpr int kImageHeight = 256;
}

ScoreUI::ScoreUI(int handle, const std::weak_ptr<Score> score) :
	UIBase(UIData::Score,handle),
	m_viewScore(0),
	m_viewMaxScore(0),
	m_score(score)
{
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
	if (m_viewScore != m_viewMaxScore)
	{
		auto score = MathSub::Lerp(static_cast<float>(m_viewScore), static_cast<float>(m_viewMaxScore), 0.5f);
		if (score + 1.0f >= static_cast<float>(m_viewMaxScore))
		{
			score = m_viewMaxScore;
		}
		m_viewScore = score;
	}
}

void ScoreUI::Draw() const
{
	DrawFormatString((Game::kScreenWidth / 2.0f) - 100.0f, 50.0f, 0xffffff, "SCORE : %6d", m_viewScore);
	//取り出す桁の位
	std::list<int> m_digits;
	//取り出す値
	int score = m_viewScore;
	while (score > 0)
	{
		//取り出して保存
		m_digits.emplace_back(score % 10);
		//桁を下げる
		score = score / 10;
	}
	//for(auto m_digits)
	//{
	//	//余りから数字を確認する
	//	auto num = m_viewScore % 10 * digit;
	//	//切り取るを計算する
	//	int sizeX, sizeY;
	//	GetGraphSize(m_handle, &sizeX, &sizeY);//画像サイズ
	//	int cutX = num % (sizeX / kImageWidth);//横
	//	int cutY = num / (sizeX / kImageWidth);//縦
	//	//描画
	//	DrawRectRotaGraphFast((Game::kScreenWidth / 2.0f) - i * 30, 50.0f,
	//		kImageWidth * cutX,
	//		kImageHeight * cutY,
	//		kImageWidth, kImageHeight,
	//		0.1f, 0.0f, m_handle, true, false);
	//	//桁をあげる
	//	digit *= 10;
	//}
}

void ScoreUI::End()
{
	DeleteGraph(m_handle);
}
