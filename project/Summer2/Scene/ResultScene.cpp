#include <DxLib.h>
#include <memory>
#include "ResultScene.h"
#include "TitleScene.h"
#include  "../General/Input.h"
#include "SceneController.h"

#if _DEBUG
//�f�o�b�O���[�h
#include "DebugScene.h"
#endif

namespace
{

}

ResultScene::ResultScene(SceneController& controller):
	SceneBase(controller)
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Init()
{
	//�Ȃ�
}

void ResultScene::Update(Input& input)
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
	if (input.IsTrigger("Pause"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
		return;
	}
}

void ResultScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Result Scene", 0xffffff);
	DrawString(0, 16, "[D]�L�[�� Debug Scene", 0xffffff);
#endif
}

void ResultScene::End()
{
}
