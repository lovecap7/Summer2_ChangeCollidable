#include "GameManager.h"
#include "../General/Input.h"
#include <DxLib.h>
#include "../General/game.h"
#include "Actors/ActorManager.h"
#include "Actors/Character/Player/Player.h"
#include "UI/UIManager.h"
#include "../General/Collision/Physics.h"
#include "../Game/Camera/Camera.h"
#include "../Game/GameRule/Score.h"
#include <cassert>

GameManager::GameManager(Stage::StageIndex index)
{
	//UI�}�l�[�W���[
	m_uiManager = std::make_shared<UIManager>();
	//�J�����̏�����
	m_camera = std::make_shared<Camera>();
	//�A�N�^�[�}�l�[�W���[
	m_actorManager = std::make_shared<ActorManager>(index, m_uiManager);
	//�X�R�A
	m_score = std::make_shared<Score>();
	m_uiManager->CreateScoreUI(m_score);
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	//UI�̏�����
	m_uiManager->Init();
	//�A�N�^�[�}�l�[�W���[�̏�����
	m_actorManager->Init();
	//�J�����̏�����
	m_camera->Init();
}

void GameManager::Update()
{
	//�f�o�b�O�ňꎞ��~����ĂȂ��Ȃ�
	auto& input = Input::GetInstance();
#if _DEBUG
	if (!m_isUpdateStop || (input.IsTrigger("StopUpdate") && m_isUpdateStop))
#endif
	{
		//�A�N�^�[�̍X�V
		m_actorManager->Update(m_camera,m_score);
		//UI�̍X�V
		m_uiManager->Update(m_actorManager);
		//�J�����̍X�V
		m_camera->Update(m_actorManager);
		//�{�X��|�����Ƃ�
		if (m_actorManager->GetBoss().expired())
		{
			//�v���C���[�̏����A�j���[�V�������I��������
			if (m_actorManager->GetPlayer().lock()->IsFinishClearAnim())
			{
				m_isGameClear = true;
			}
		}
		//�v���C���[�����S�����ۂ̏���
		else if (m_actorManager->GetPlayer().expired())
		{
			m_isGameover = true;
		}
	}
}

void GameManager::Draw() const
{
#if _DEBUG
	DrawString(0, 0, "Stage1 Scene", 0xffffff);
	DrawString(0, 16, "[D]�L�[�� Debug Scene", 0xffffff);

	for (int z = -500; z <= 500; z += 100)
	{
		DrawLine3D(VGet(-500, 0, z), VGet(500, 0, z), 0xff0000);
	}
	for (int x = -500; x <= 500; x += 100)
	{
		DrawLine3D(VGet(x, 0, -500), VGet(x, 0, 500), 0x0000ff);
	}
	VECTOR screenPos = ConvWorldPosToScreenPos(VGet(500, 0, 0));
	DrawString(screenPos.x, screenPos.y, "X+", 0xffffff);
	screenPos = ConvWorldPosToScreenPos(VGet(-500, 0, 0));
	DrawString(screenPos.x, screenPos.y, "X-", 0xffffff);
	screenPos = ConvWorldPosToScreenPos(VGet(0, 0, 500));
	DrawString(screenPos.x, screenPos.y, "Z+", 0xffffff);
	screenPos = ConvWorldPosToScreenPos(VGet(0, 0, -500));
	DrawString(screenPos.x, screenPos.y, "Z-", 0xffffff);
#endif
	//�A�N�^�[�̕`��
	m_actorManager->Draw();
	//UI�̕`��
	m_uiManager->Draw();
}

void GameManager::End()
{
	//�A�N�^�[�}�l�[�W���[�̏I��
	m_actorManager->End();
	//UI�}�l�[�W���[�̏I��
	m_uiManager->End();
}
