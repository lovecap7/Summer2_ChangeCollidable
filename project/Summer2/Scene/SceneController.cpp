#include "SceneController.h"
#include <memory>
#include<cassert>
//�p�����̊֐����F�����Ă����
#include "TitleScene.h"
#if _DEBUG
#include "DebugScene.h"
#endif

SceneController::SceneController()
{
#if _DEBUG
	//��ԍŏ��̃V�[�������͊��蓖�Ă�
	//�������g�̃C���X�^���X��n���Ă�����
	ChangeScene(std::make_shared<DebugScene>(*this));
#else
	//��ԍŏ��̃V�[�������͊��蓖�Ă�
	//�������g�̃C���X�^���X��n���Ă�����
	ChangeScene(std::make_shared<TitleScene>(*this));
#endif

}

void SceneController::Update()
{
	//�Ō�Ƀv�b�V��(���ꂽ)�V�[���̂ݍX�V�������s��(���̃V�[���͍X�V�̓X�g�b�v)
	m_scenes.back()->Update();
}

void SceneController::Draw()
{
	//�Â��V�[������`�悳���̂ōŌ�ɓ��ꂽ�V�[�����O�ɕ`�悳���
	for (auto& scenes : m_scenes)
	{
		scenes->Draw();
	}
}

void SceneController::ChangeScene(std::shared_ptr<SceneBase> scene)
{
	//��(�ŏ��̏���)
	if (m_scenes.empty())
	{
		m_scenes.emplace_back(scene);
	}
	else
	{
		//�I������
		m_scenes.back()->End();
		//�擪�̃V�[�����؂�ւ��
		m_scenes.back() = scene;
	}
	//����������
	m_scenes.back()->Init();
}

void SceneController::ChangeBaseScene(std::shared_ptr<SceneBase> scene)
{
	//��̏ꍇ�G���[
	assert(!m_scenes.empty());
	//�I������
	m_scenes.front()->End();
	m_scenes.pop_front();
	m_scenes.emplace_front(scene);
	//����������
	m_scenes.front()->Init();
}

void SceneController::PushScene(std::shared_ptr<SceneBase> scene)
{
	m_scenes.emplace_back(scene);
	m_scenes.back()->Init();	//����������
}

void SceneController::PopScene()
{
	//���s����V�[�����Ȃ��Ȃ�̂ŋ����Ȃ�
	if (m_scenes.size() == 1)return;
	m_scenes.back()->End();	//�I������
	m_scenes.pop_back();	//��������菜��
}
