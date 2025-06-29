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
	//UI���󂯎��
	stageSetup->MoveUIPointer(m_uis);
	for (auto& ui : m_uis)
	{
		ui->Init();
	}
}

void UIManager::Update()
{
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
}

void UIManager::AddUI(std::shared_ptr<UIBase> ui)
{
	//���łɓo�^����Ă���Ȃ炵�Ȃ�
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())return;
	//UI�̏�����
	ui->Init();
	//�ǉ�
	m_uis.emplace_back(ui);
}
