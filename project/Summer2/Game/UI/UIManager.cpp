#include "UIManager.h"
#include "UIBase.h"
#include "../Actors/ActorManager.h"
//�v���C���[
#include "PlayerUI/PlayerHPUI.h"
#include "PlayerUI/PlayerUltGageUI.h"
//�{�X
#include "EnemyUI/BossHPUI.h"
//�G
#include "EnemyUI/EnemyHPUI.h"
//�X�R�A
#include "ScoreUI.h"
//�^�C�}�[
#include "TimerUI.h"
UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::Init()
{
}

void UIManager::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//�폜�\���UI�폜
	CheckDelete();
	//�X�V
	for (auto& ui : m_uis)
	{
		ui->Update(actorManager);
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

void UIManager::Restart()
{
	End();
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

void UIManager::CreatePlayerUI(const std::weak_ptr<Player> player)
{
	AddUI(std::make_shared<PlayerHPUI>(player));
	AddUI(std::make_shared<PlayerUltGageUI>(player));
}

void UIManager::CreateBossUI(const std::weak_ptr<EnemyBase> boss)
{
	AddUI(std::make_shared<BossHPUI>(boss));
}

void UIManager::CreateEnemyUI(const std::weak_ptr<EnemyBase> enemy)
{
	AddUI(std::make_shared<EnemyHPUI>(enemy));
}

void UIManager::CreateScoreUI(const std::weak_ptr<Score> score)
{
	AddUI(std::make_shared<ScoreUI>(score));
}

void UIManager::CreateTimerUI(const std::weak_ptr<Timer> timer)
{
	AddUI(std::make_shared<TimerUI>(timer));
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
