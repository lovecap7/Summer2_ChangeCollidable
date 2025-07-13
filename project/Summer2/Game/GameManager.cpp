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
#include "../Game/GameRule/Timer.h"
#include <cassert>

namespace
{
	//�V���h�E�}�b�v�̕`��T�C�Y
	constexpr int kShadowMapWidth = 1024;
	constexpr int kShadowMapHeight = 1024;
	//���C�g�̌���
	const VECTOR kLightDir = { 0.0f, -1.0f, 0.5f };
	//�V���h�E�}�b�v�͈̔�
	constexpr float kShadowMapHorizon = 2000.0f;
	constexpr float kShadowMapVerticalMin = -1.0f;
	constexpr float kShadowMapVerticalMax = 1000.0f;
}

GameManager::GameManager(Stage::StageIndex index):
	m_isGameover(false),
	m_isGameClear(false)
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
	//�^�C�}�[
	m_timer = std::make_shared<Timer>();
	m_uiManager->CreateTimerUI(m_timer);
	//�V���h�E�}�b�v�̏���
	InitShadow();
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
		//�^�C�}�[
		m_timer->Update();
		//UI�̍X�V
		m_uiManager->Update(m_actorManager);
		//�J�����̍X�V
		m_camera->Update(m_actorManager);
		//�{�X��|�����Ƃ�
		if (m_actorManager->GetBoss().expired() && !m_isGameClear)
		{
			//�v���C���[�̏����A�j���[�V�������I��������
			if (m_actorManager->GetPlayer().lock()->IsFinishClearAnim())
			{
				//�^�C�}�[���X�R�A�ɉ��Z
				m_score->AddTimeScore(m_timer->GetTime());
				//�N���A
				m_isGameClear = true;
			}
		}
		//�v���C���[�����S�����ۂ̏���
		else if (m_actorManager->GetPlayer().expired() && !m_isGameover)
		{
			//�Q�[���I�[�o�[
			m_isGameover = true;
		}
	}
	//�V���h�E�}�b�v�ɕ`�悷��͈͂�ݒ�
	//�J�����̎��͂̂�
	UpdateShadowDrawArea();
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
	//�V���h�E�}�b�v�ւ̕`��̏���
	ShadowMap_DrawSetup(m_shadowMapHandle);
	//�V���h�E�}�b�v�ւ̃A�N�^�[�̕`��
	m_actorManager->Draw();
	//�V���h�E�}�b�v�ւ̕`����I��
	ShadowMap_DrawEnd();
	//�`��Ɏg�p����V���h�E�}�b�v��ݒ�
	SetUseShadowMap(0, m_shadowMapHandle);
	//�A�N�^�[�̕`��
	m_actorManager->Draw();
	//UI�̕`��
	m_uiManager->Draw();
	//�`��Ɏg�p����V���h�E�}�b�v�̐ݒ������
	SetUseShadowMap(0, -1);
}

void GameManager::End()
{
	//�A�N�^�[�}�l�[�W���[�̏I��
	m_actorManager->End();
	//UI�}�l�[�W���[�̏I��
	m_uiManager->End();
	//�V���h�E�}�b�v�̍폜
	DeleteShadowMap(m_shadowMapHandle);
}

void GameManager::InitShadow()
{
	//�f�B���N�V���i�����C�g
	ChangeLightTypeDir(kLightDir);
	//�V���h�E�}�b�v�n���h���̍쐬
	m_shadowMapHandle = MakeShadowMap(kShadowMapWidth, kShadowMapHeight);
	//�V���h�E�}�b�v���z�肷�郉�C�g�̕������Z�b�g
	SetShadowMapLightDirection(m_shadowMapHandle, kLightDir);
}

void GameManager::UpdateShadowDrawArea()
{
	auto shadowMinPos = m_camera->GetPos();
	shadowMinPos.x += -kShadowMapHorizon;
	shadowMinPos.y = kShadowMapVerticalMin;
	shadowMinPos.z += -kShadowMapHorizon;
	auto shadowMaxPos = m_camera->GetPos();
	shadowMaxPos.x += kShadowMapHorizon;
	shadowMaxPos.y = kShadowMapVerticalMax;
	shadowMaxPos.z += kShadowMapHorizon;
	SetShadowMapDrawArea(m_shadowMapHandle, shadowMinPos.ToDxLibVector(), shadowMaxPos.ToDxLibVector());
}
