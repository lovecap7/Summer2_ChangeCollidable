#include "UIManager.h"
#include "UIBase.h"
#include "../Actors/ActorManager.h"
#include <cassert>
//プレイヤー
#include "PlayerUI/PlayerHPUI.h"
#include "PlayerUI/PlayerUltGageUI.h"
//ボス
#include "EnemyUI/BossHPUI.h"
//敵
#include "EnemyUI/EnemyHPUI.h"
//スコア
#include "GameScoreUI.h"
//タイマー
#include "TimerUI.h"

namespace
{
	constexpr int kFontSize16 = 16;
	constexpr int kFontSize32 = 32;
	constexpr int kFontSize48 = 48;
	constexpr int kFontSize64 = 64;
	constexpr int kFontSize80 = 80;
	constexpr int kFontSize96 = 96;
}

void UIManager::Entry(std::shared_ptr<UIBase> ui)
{
	//すでに登録されているならしない
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())return;
	//登録
	m_uis.emplace_back(ui);
}

void UIManager::Exit(std::shared_ptr<UIBase> ui)
{
	//登録されていないならしない
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it == m_uis.end())return;
	m_uis.erase(it);
}
void UIManager::Init()
{
	//ハンドルロード
	LoadHandle();
	//更新と描画フラグ
	m_isUpdate = true;
	m_isDraw = true;
}

void UIManager::Update()
{
	//更新をするか
	if (!m_isUpdate)return;
	//削除予定のUI削除
	CheckDelete();
	//更新
	for (auto& ui : m_uis)
	{
		ui->Update();
	}
}

void UIManager::BackDraw() const
{
	//描画をするか
	if (!m_isDraw)return;
	for (auto& ui : m_uis)
	{
		//後ろ描画じゃないなら
		if (ui->IsFront())continue;
		ui->Draw();
	}
}

void UIManager::FrontDraw() const
{
	//描画をするか
	if (!m_isDraw)return;
	for (auto& ui : m_uis)
	{
		//前描画じゃないなら
		if (!ui->IsFront())continue;
		ui->Draw();
	}
}

void UIManager::End()
{
	//すべて削除
	AllDeleteUIs();
	//ハンドル削除
	AllDeleteHandle();
}

void UIManager::Reset()
{
	//UIのみすべて削除
	AllDeleteUIs();
}

void UIManager::CreatePlayerUI(const std::weak_ptr<Player> player)
{
	Entry(std::make_shared<PlayerHPUI>(player));
	Entry(std::make_shared<PlayerUltGageUI>(player));
}

std::weak_ptr<BossHPUI> UIManager::CreateBossUI(const std::weak_ptr<EnemyBase> boss)
{
	std::shared_ptr<BossHPUI> ui = std::make_shared<BossHPUI>(boss);
	Entry(ui);
	return ui;
}

void UIManager::CreateEnemyUI(const std::weak_ptr<EnemyBase> enemy)
{
	Entry(std::make_shared<EnemyHPUI>(enemy));
}

void UIManager::CreateGameScoreUI(const std::weak_ptr<Score> score)
{
	Entry(std::make_shared<GameScoreUI>(score));
}

void UIManager::CreateTimerUI(const std::weak_ptr<Timer> timer)
{
	Entry(std::make_shared<TimerUI>(timer));
}

int UIManager::GetImageHandle(const std::string& name) const
{
	auto it = m_imageHandles.find(name);
	if (it == m_imageHandles.end()) {
		//ログを出すかデフォルト値を返す
		assert(false && "ImageHandles not found");
		return -1;
	}
	return it->second;
}

int UIManager::GetTextHandle(const std::string& name) const
{
	auto it = m_textHandles.find(name);
	if (it == m_textHandles.end()) {
		//ログを出すかデフォルト値を返す
		assert(false && "TextHandle not found");
		return -1;
	}
	return it->second;
}

void UIManager::LoadHandle()
{
	//画像ハンドル
	m_imageHandles["Score"] = { LoadGraph(L"Data/UI/Number.png") };
	m_imageHandles["Timer"] = { LoadGraph(L"Data/UI/Number_Stone.png") };
	//メニュー
	m_imageHandles["Pause"] = { LoadGraph(L"Data/UI/Menu/Pause.png") };
	m_imageHandles["ReturnGame"] = { LoadGraph(L"Data/UI/Menu/ReturnGame.png") };
	m_imageHandles["RestartGame"] = { LoadGraph(L"Data/UI/Menu/RestartGame.png") };
	m_imageHandles["Option"] = { LoadGraph(L"Data/UI/Menu/Option.png") };
	m_imageHandles["SelectStage"] = { LoadGraph(L"Data/UI/Menu/SelectStage.png") };
	//ロードに成功したかチェック
	for (auto& [key, value] : m_imageHandles) {
		assert(value >= 0);
	}
	//テキストハンドル
	m_textHandles["メイリオ16"] = { CreateFontToHandle(L"Meiryo", kFontSize16, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["メイリオ32"] = { CreateFontToHandle(L"Meiryo", kFontSize32, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["メイリオ48"] = { CreateFontToHandle(L"Meiryo", kFontSize48, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["メイリオ64"] = { CreateFontToHandle(L"Meiryo", kFontSize64, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["メイリオ80"] = { CreateFontToHandle(L"Meiryo", kFontSize80, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["メイリオ96"] = { CreateFontToHandle(L"Meiryo", kFontSize96, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["MSPゴシック16"] = { CreateFontToHandle(L"MS PGothic", kFontSize16, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["MSPゴシック32"] = { CreateFontToHandle(L"MS PGothic", kFontSize32, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["MSPゴシック48"] = { CreateFontToHandle(L"MS PGothic", kFontSize48, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["MSPゴシック64"] = { CreateFontToHandle(L"MS PGothic", kFontSize64, 5, DX_FONTTYPE_ANTIALIASING) };
	//ロードに成功したかチェック
	for (auto& [key, value] : m_textHandles) {
		assert(value >= 0);
	}
}

void UIManager::AllDeleteUIs()
{
	m_uis.clear();
}

void UIManager::AllDeleteHandle()
{
	//画像の削除
	for (auto& [key, value] : m_imageHandles) {
		if (value >= 0)
		{
			auto result = DeleteGraph(value);
			assert(result == 0);
		}
	}
	m_imageHandles.clear();
	//テキストの削除
	for (auto& [key, value] : m_textHandles) {
		DeleteFontToHandle(value);
	}
	m_textHandles.clear();
}

void UIManager::CheckDelete()
{
	auto remIt = std::remove_if(m_uis.begin(), m_uis.end(), [](std::shared_ptr<UIBase> ui) {
		return  ui->IsDelete();
		});
	m_uis.erase(remIt, m_uis.end());//削除
}
