#include "SelectStageScene.h"
#include "StageScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <memory>
#include <DxLib.h>
#if _DEBUG
//�f�o�b�O���[�h
#include "DebugScene.h"
#endif

SelectStageScene::SelectStageScene(SceneController& controller):
	SceneBase(controller)
{
	//�Ȃ�
}

SelectStageScene::~SelectStageScene()
{
}

void SelectStageScene::Init()
{
	//�Ȃ�
}

void SelectStageScene::Update(Input& input)
{
#if _DEBUG
	//�f�o�b�O�V�[��
	if (input.IsTrigger("SceneChange"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}
#endif
	if (input.IsTrigger("A"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<StageScene>(m_controller));
		return;
	}
	if (input.IsTrigger("B"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<StageScene>(m_controller));
		return;
	}
	if (input.IsTrigger("X"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<StageScene>(m_controller));
		return;
	}
}

void SelectStageScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "SelectStage Scene", 0xffffff);
	DrawString(0, 16, "[D]�L�[�� Debug Scene", 0xffffff);
	DrawString(0, 32, "A Bottun : Stage 1", 0xffff00);
	DrawString(0, 48, "B Bottun : Stage 2", 0xffff00);
	DrawString(0, 64, "X Bottun : Stage 3", 0xffff00);
#endif
}

void SelectStageScene::End()
{
}
