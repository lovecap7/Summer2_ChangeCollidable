#include "UIManager.h"
#include "UIBase.h"
#include "../Actors/ActorManager.h"
#include <cassert>
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
	//�n���h�����[�h
	LoadHandle();
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
	//���ׂč폜
	AllDeleteUIs();
	//�n���h���폜
	AllDeleteHandle();
}

void UIManager::Restart()
{
	//UI�݂̂��ׂč폜
	AllDeleteUIs();
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
	AddUI(std::make_shared<PlayerHPUI>(-1,player));
	AddUI(std::make_shared<PlayerUltGageUI>(-1, player));
}

void UIManager::CreateBossUI(const std::weak_ptr<EnemyBase> boss)
{
	AddUI(std::make_shared<BossHPUI>(-1, boss));
}

void UIManager::CreateEnemyUI(const std::weak_ptr<EnemyBase> enemy)
{
	AddUI(std::make_shared<EnemyHPUI>(-1, enemy));
}

void UIManager::CreateScoreUI(const std::weak_ptr<Score> score)
{
	AddUI(std::make_shared<ScoreUI>(m_handles["Score"], score));
}

void UIManager::CreateTimerUI(const std::weak_ptr<Timer> timer)
{
	AddUI(std::make_shared<TimerUI>(-1, timer));
}

void UIManager::LoadHandle()
{
	m_handles["Score"] = { LoadGraph("Data/UI/Number.png") };
	
	//���[�h�ɐ����������`�F�b�N
	for (auto& [key, value] : m_handles) {
		assert(value >= 0);
	}
}

void UIManager::AllDeleteUIs()
{
	for (auto& ui : m_uis)
	{
		ui->End();
	}
	m_uis.clear();
}

void UIManager::AllDeleteHandle()
{
	for (auto& [key, value] : m_handles) {
		if (value >= 0)
		{
			auto result = DeleteGraph(value);
			assert(result == 0);
		}
	}
	m_handles.clear();
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
