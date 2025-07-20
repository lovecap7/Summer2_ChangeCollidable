#include "TutorialUI.h"
#include "UIManager.h"
#include "../../General/game.h"
#include "../../General/StringUtil.h"
#include <DxLib.h>

namespace
{
	//四角の座標
	constexpr int kSpaceLeftPos = Game::kScreenWidth / 2 + 100;
	constexpr int kSpaceUpPos = 60;
	constexpr int kSpaceRightPos = Game::kScreenWidth;
	constexpr int kSpaceDownPos = 135;
	//改行の数だけ四角の下の座標を下げてく
	constexpr int kSpaceDownSize = 16;
	//”チュートリアル”の表示位置
	constexpr int kTutorialPosX = kSpaceLeftPos + 20;
	constexpr int kTutorialPosY = kSpaceUpPos + 10;
	//１行に入る文字列の数
	constexpr int kOneLineMaxNum = 30;
	//テキストの座標
	constexpr int kTextPosX = kSpaceLeftPos + 30;
	constexpr int kTextPosY = kTutorialPosY + 30;
	//テキストの表示速度
	constexpr int kTextViewSpeed = 2;
}

TutorialUI::TutorialUI():
	UIBase(UIManager::GetInstance().GetTextHandle("メイリオ")),
	m_countFrame(0),
	m_chatCount(0),
	text{ L"チュートリアルのテキストを表示します。" }
{
	text = InsertNewLines(text, kOneLineMaxNum);
}

TutorialUI::~TutorialUI()
{
}

void TutorialUI::Update()
{
	++m_countFrame;
	if (m_countFrame % kTextViewSpeed == 0 && m_chatCount < static_cast<int>(text.size()))
	{
		++m_chatCount;
	}
}

void TutorialUI::Draw() const
{
	//チュートリアルのスペース
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	//改行の数
	auto lineNum = WStringLineNum(text);
	DrawBox(kSpaceLeftPos, kSpaceUpPos, kSpaceRightPos, kSpaceDownPos + kSpaceDownSize * lineNum, 0x444444,true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//テキスト
	DrawStringToHandle(kTutorialPosX, kTutorialPosY, L"<チュートリアル>", 0xffffff, m_handle);
	DrawStringToHandle(kTextPosX, kTextPosY, text.substr(0, m_chatCount).c_str(), 0xffffff, m_handle);
	DrawFormatString(0, 10, 0xffff00, L"SIZE : %d", (int)text.size());
}
