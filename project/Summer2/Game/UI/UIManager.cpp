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
	//���łɓo�^����Ă���Ȃ�o�^���Ȃ�
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())
	{
		return;
	}
	//UI���Z�b�g
	m_uis.emplace_back(ui);
}

void UIManager::Exit(std::shared_ptr<UIBase> ui)
{
	//���łɓo�^����Ă���Ȃ�Â�UI������
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())
	{
		//�폜
		m_uis.erase(it);
	}
}

void UIManager::Update()
{
	if (m_uis.empty())return;//��Ȃ牽�����Ȃ�

	//UI�̍X�V����
	for (auto& ui : m_uis)
	{
		ui->Update();
	}
}

void UIManager::Draw() const
{
	if (m_uis.empty())return;//��Ȃ牽�����Ȃ�

	//UI�̕`�揈��
	for (auto& ui : m_uis)
	{
		ui->Draw();
	}
}
