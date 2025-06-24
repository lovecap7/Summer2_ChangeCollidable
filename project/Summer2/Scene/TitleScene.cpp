#include "TitleScene.h"
#include "Dxlib.h"
#include  "../General/Input.h"
#include "SceneController.h"
#include "SelectStageScene.h"
#include <memory>
#if _DEBUG
//�f�o�b�O���[�h
#include "DebugScene.h"
#endif

namespace
{
}

TitleScene::TitleScene(SceneController& controller):
	SceneBase(controller)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	//�Ȃ�
}

void TitleScene::Update(Input& input)
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
		m_controller.ChangeScene(std::make_shared<SelectStageScene>(m_controller));
		return;
	}
}

void TitleScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Title Scene", 0xffffff);
	DrawString(0, 16, "[D]�L�[�� Debug Scene", 0xffffff);
#endif
}

void TitleScene::End()
{
}
