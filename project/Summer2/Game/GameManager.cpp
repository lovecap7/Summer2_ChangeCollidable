#include "GameManager.h"
#include "../General/Input.h"
#include <DxLib.h>
#include <vector>
#include "../General/game.h"
#include "StageSetup.h"
#include "Actors/ActorManager.h"
#include "Actors/Player/Player.h"
//UI
#include "../Game/UI/UIManager.h"
#include "../Game/UI/UIPlayerHP.h"
#include "../Game/UI/UIPlayerUltGage.h"
//�J����
#include "../Game/Camera/Camera.h"
#include <cassert>

namespace
{
	const Vector3 kCameraPos = { 0.0f, 600.0f, -600.0f };
}

GameManager::GameManager()
{
	//�X�e�[�W���쐬
	m_stageSetup = std::make_shared<StageSetup>();
	m_stageSetup->MovePlayerPointer(m_player);
	//�A�N�^�[�}�l�[�W���[
	m_actorManager = std::make_shared<ActorManager>(m_player);
	//�J�����̏�����
	m_camera = std::make_unique<Camera>(kCameraPos, m_player);
	//UI�̏���
	m_uiManager = std::make_shared<UIManager>();
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	//�Z�b�g�A�b�v�N���X����A�N�^�[���󂯎��
	//����������
	m_actorManager->Init(m_stageSetup);
	//UI�̍쐬
	//�v���C���[�̗̑͂ƕK�E�Q�[�W
	auto playerHpUI = std::make_shared<UIPlayerHP>(m_player->GetHurtPoint());
	auto playerUltUI = std::make_shared<UIPlayerUltGage>(m_player->GetUltGage());
	m_uiManager->Entry(playerHpUI);
	m_uiManager->Entry(playerUltUI);
}

void GameManager::Update(Input& input)
{
	//�f�o�b�O�ňꎞ��~����ĂȂ��Ȃ�
#if _DEBUG
	if (!m_isUpdateStop || (input.IsTrigger("StopUpdate") && m_isUpdateStop))
#endif
	{
		//�A�N�^�[�̍X�V����
		m_actorManager->Update(input, m_camera, m_uiManager);
		//�J�����̍X�V
		m_camera->Update();
		//UI�̍X�V
		m_uiManager->Update();
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
	//�I������
	m_stageSetup->End();
	//�o�^����
	m_actorManager->End();
}
