#include "SaveDataUI.h"
#include "UIManager.h"
#include "../../General/game.h"
#include <DxLib.h>
#include "../../SaveData/SaveDataManager.h"

namespace
{
	//背景位置
	constexpr int kTextBackPosX = Game::kScreenCenterX + 300;
	constexpr int kTextBackPosY = Game::kScreenCenterY;
	//時間位置
	constexpr int kTextBasePosX = Game::kScreenCenterX;
	constexpr int kTextClearStage1PosY = Game::kScreenCenterY - 81;
	constexpr int kTextClearStage2PosY = Game::kScreenCenterY - 39;
	constexpr int kTextClearStage3PosY = Game::kScreenCenterY + 3;
	constexpr int kTextPlayTimePosY = Game::kScreenCenterY + 43;
	constexpr int kTextLastSaveTimeAndDatePosY = Game::kScreenCenterY + 85;
	//分
	constexpr int kMinutes = 3600;
	//時
	constexpr int kHourss = 60;
}

SaveDataUI::SaveDataUI():
	UIBase(),
	m_backHandle(UIManager::GetInstance().GetImageHandle("SaveDataBack")),
	m_textHandle(UIManager::GetInstance().GetTextHandle("メイリオ32"))
{

}

SaveDataUI::~SaveDataUI()
{
}

void SaveDataUI::Update()
{
}

void SaveDataUI::Draw() const
{
	if (!m_isDraw)return;
	DrawRotaGraph(kTextBackPosX, kTextBackPosY, 1.0, 0.0, m_backHandle, true);
	//クリアしたステージ
	DrawClearStage(Vector2{ kTextBasePosX ,kTextClearStage1PosY },Stage::StageIndex::Stage1);
	DrawClearStage(Vector2{ kTextBasePosX ,kTextClearStage2PosY },Stage::StageIndex::Stage2);
	DrawClearStage(Vector2{ kTextBasePosX ,kTextClearStage3PosY },Stage::StageIndex::Stage3);

	//トータルプレイ時間
	auto totalTime = SaveDataManager::GetInstance().GetTotalPlayTime();
	auto minutes = totalTime / kMinutes;
	auto hours	 = minutes	 / kHourss;
	std::wstring timeText = L"総プレイ時間 : " + std::to_wstring(hours) + L"時間" + std::to_wstring(minutes) + L"分";
	DrawStringToHandle(kTextBasePosX, kTextPlayTimePosY, timeText.c_str(), 0x000000, m_textHandle);
	auto lastSaveText = L"前回のプレイ : " + SaveDataManager::GetInstance().GetLastSaveTimeAndDate();
	//日付と時間
	DrawStringToHandle(kTextBasePosX, kTextLastSaveTimeAndDatePosY, lastSaveText.c_str(), 0x000000, m_textHandle);
}

void SaveDataUI::DrawClearStage(Vector2 pos, Stage::StageIndex stageIndex) const
{
	auto isClear = SaveDataManager::GetInstance().IsClearStage(stageIndex);
	std::wstring stageClearText = L"ステージ" + std::to_wstring(static_cast<int>(stageIndex) + 1);
	if (isClear)
	{
		stageClearText += L" : クリア";
	}
	else
	{
		stageClearText += L" : 未クリア";
	}
	DrawStringToHandle(pos.x, pos.y, stageClearText.c_str(), 0x000000, m_textHandle);
}
