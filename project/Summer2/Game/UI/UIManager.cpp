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
	//�폜�\���UI�폜
	CheckDelete();
	//�X�V
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
	//���łɓo�^����Ă���Ȃ炵�Ȃ�
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())return;
	//UI�̏�����
	ui->Init();
	//�ǉ�
	m_uis.emplace_back(ui);
}

void UIManager::CheckDelete()
{
	auto remIt = std::remove_if(m_uis.begin(), m_uis.end(), [](std::shared_ptr<UIBase> ui) {
		bool isDead = ui->IsDelete();//�`�F�b�N
		if (isDead)
		{
			//UI�̏I������
			ui->End();
		}
		return isDead;
		});
	m_uis.erase(remIt, m_uis.end());//�폜
}
