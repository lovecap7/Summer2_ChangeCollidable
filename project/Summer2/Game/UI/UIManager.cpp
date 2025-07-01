#include "UIManager.h"
#include "UIBase.h"
#include "../StageSetup.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::Init(std::unique_ptr<StageSetup>& stageSetup)
{
	//UIを受け取る
	stageSetup->MoveUIPointer(m_uis);
	for (auto& ui : m_uis)
	{
		ui->Init();
	}
}

void UIManager::Update()
{
	//削除予定のUI削除
	CheckDelete();
	//更新
	for (auto& ui : m_uis)
	{
		ui->Update();
	}
}

void UIManager::Draw() const
{
	for (auto& ui : m_uis)
	{
		ui->Draw();
	}
}

void UIManager::End()
{
	for (auto& ui : m_uis)
	{
		ui->End();
	}
	m_uis.clear();
}

void UIManager::AddUI(std::shared_ptr<UIBase> ui)
{
	//すでに登録されているならしない
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())return;
	//UIの初期化
	ui->Init();
	//追加
	m_uis.emplace_back(ui);
}

void UIManager::CheckDelete()
{
	auto remIt = std::remove_if(m_uis.begin(), m_uis.end(), [](std::shared_ptr<UIBase> ui) {
		bool isDead = ui->IsDelete();//チェック
		if (isDead)
		{
			//UIの終了処理
			ui->End();
		}
		return isDead;
		});
	m_uis.erase(remIt, m_uis.end());//削除
}
