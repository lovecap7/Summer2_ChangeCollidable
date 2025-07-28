#include "ResultScoreNameUI.h"
#include <cassert>

ResultScoreNameUI::ResultScoreNameUI():
	UIBase(true)
{
	m_handleMap["TotalScore"] = LoadGraph(L"Data/UI/Result/TotalScore.png");
	for (const auto& handle : m_handleMap)
	{
		assert(handle.second != -1);
	}
}

ResultScoreNameUI::~ResultScoreNameUI()
{
	//ƒnƒ“ƒhƒ‹‚ð•Â‚¶‚é
	for (const auto& handle : m_handleMap)
	{
		DeleteGraph(handle.second);
	}
	m_handleMap.clear();
}

void ResultScoreNameUI::Update()
{
}

void ResultScoreNameUI::Draw() const
{
	//•`‰æ
	DrawRotaGraphFast(400, 150, 1.0, 0, m_handleMap.at("TotalScore"),true);
}
