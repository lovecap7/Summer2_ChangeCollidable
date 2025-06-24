#include "UIManager.h"
#include "UIBase.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::Entry(std::shared_ptr<UIBase> ui)
{
	//‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚é‚È‚ç“o˜^‚µ‚È‚¢
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())
	{
		return;
	}
	//UI‚ðƒZƒbƒg
	m_uis.emplace_back(ui);
}

void UIManager::Exit(std::shared_ptr<UIBase> ui)
{
	//‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚é‚È‚çŒÃ‚¢UI‚ðÁ‚·
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())
	{
		//íœ
		m_uis.erase(it);
	}
}

void UIManager::Update()
{
	if (m_uis.empty())return;//‹ó‚È‚ç‰½‚à‚µ‚È‚¢

	//UI‚ÌXVˆ—
	for (auto& ui : m_uis)
	{
		ui->Update();
	}
}

void UIManager::Draw() const
{
	if (m_uis.empty())return;//‹ó‚È‚ç‰½‚à‚µ‚È‚¢

	//UI‚Ì•`‰æˆ—
	for (auto& ui : m_uis)
	{
		ui->Draw();
	}
}
