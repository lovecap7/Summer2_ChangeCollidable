#include "TimerUI.h"
#include "../GameRule/Timer.h"
#include "../../General/game.h"
#include "../Actors/ActorManager.h"
#include <DxLib.h>

namespace
{
	//スコアの桁数
	constexpr int kDigitNum = 2;
	//大きさ
	constexpr float kScale = 0.2f;
	//幅
	constexpr int kImageWidth = 256;
	constexpr int kImageHeight = 256;
	//タイマーのX座標
	constexpr int kMinSecPosX = (Game::kScreenWidth - 100);
	constexpr int kSecPosX = (Game::kScreenWidth - 170);
	constexpr int kMinPosX = (Game::kScreenWidth - 240);
	//タイマーのY座標
	constexpr int kPosY = 120;
	//タイマーの1桁の幅
	constexpr int kDigitMargin = 30;
}

TimerUI::TimerUI(int handle,const std::weak_ptr<Timer> timer):
	UIBase(UIData::Timer,handle),
	m_timer(timer)
{
}

TimerUI::~TimerUI()
{
}

void TimerUI::Init()
{
}

void TimerUI::Update(const std::weak_ptr<ActorManager> actorManager)
{
	if (m_timer.expired() || actorManager.lock()->GetBoss().expired())
	{
		m_isDelete = true;
		return;
	}
}

void TimerUI::Draw() const
{
	if (m_timer.expired())return;
	auto time = m_timer.lock();
	int minSec = time->GetMillisecond();
	int sec = time->GetSeconds();
	int min = time->GetMinutes();
	//各桁
	int digits[kDigitNum];
	for (int i = 0;i < kDigitNum;++i)
	{
		//取り出して保存
		digits[i] = minSec % 10;
		//桁を下げる
		minSec = minSec / 10;
		//切り取りを計算する
		int sizeX, sizeY;
		GetGraphSize(m_handle, &sizeX, &sizeY);//画像サイズ
		int cutX = digits[i] % (sizeX / kImageWidth);//横
		int cutY = digits[i] / (sizeX / kImageWidth);//縦
		//描画
		DrawRectRotaGraphFast(kMinSecPosX - i * kDigitMargin, kPosY,
			kImageWidth * cutX,
			kImageHeight * cutY,
			kImageWidth, kImageHeight,
			kScale, 0.0f, m_handle, true, false);
	}
	for (int i = 0;i < kDigitNum;++i)
	{
		//取り出して保存
		digits[i] = sec % 10;
		//桁を下げる
		sec = sec / 10;
		//切り取りを計算する
		int sizeX, sizeY;
		GetGraphSize(m_handle, &sizeX, &sizeY);//画像サイズ
		int cutX = digits[i] % (sizeX / kImageWidth);//横
		int cutY = digits[i] / (sizeX / kImageWidth);//縦
		//描画
		DrawRectRotaGraphFast(kSecPosX - i * kDigitMargin, kPosY,
			kImageWidth * cutX,
			kImageHeight * cutY,
			kImageWidth, kImageHeight,
			kScale, 0.0f, m_handle, true, false);
	}
	for (int i = 0;i < kDigitNum;++i)
	{
		//取り出して保存
		digits[i] = min % 10;
		//桁を下げる
		min = min / 10;
		//切り取りを計算する
		int sizeX, sizeY;
		GetGraphSize(m_handle, &sizeX, &sizeY);//画像サイズ
		int cutX = digits[i] % (sizeX / kImageWidth);//横
		int cutY = digits[i] / (sizeX / kImageWidth);//縦
		//描画
		DrawRectRotaGraphFast(kMinPosX - i * kDigitMargin, kPosY,
			kImageWidth * cutX,
			kImageHeight * cutY,
			kImageWidth, kImageHeight,
			kScale, 0.0f, m_handle, true, false);
	}
}

void TimerUI::End()
{
}
