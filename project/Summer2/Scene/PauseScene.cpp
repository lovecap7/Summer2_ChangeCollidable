#include "PauseScene.h"
#include "../General/Input.h"
#include "SceneController.h"
#include<DxLib.h>
#include "../General/game.h"
#include "../General/Collision/Physics.h"

namespace {
	constexpr int kAppearInterval = 20;
	constexpr int kFrameMargin = 10;//�Q�[����ʂ���|�[�Y��ʂ܂ł̕�
}


PauseScene::PauseScene(SceneController& controller):
	SceneBase(controller),
	m_update(&PauseScene::AppearUpdate),
	m_draw(&PauseScene::ShiftingDraw),
	m_countFrame(0)
{
}

PauseScene::~PauseScene()
{
}

void PauseScene::Init()
{
	//Physics���~�߂�
	Physics::GetInstance().StopUpdate();
}

void PauseScene::Update()
{
	(this->*m_update)();
}

void PauseScene::Draw()
{
	(this->*m_draw)();
}

void PauseScene::End()
{
	//Physics���J�n
	Physics::GetInstance().StartUpdate();
}

void PauseScene::Restart()
{
}

void PauseScene::AppearUpdate()
{
	++m_countFrame;
	if (m_countFrame > kAppearInterval) 
	{
		m_update = &PauseScene::NormalUpdate;
		m_draw = &PauseScene::NormalDraw;
		return;
	}
}

void PauseScene::NormalUpdate()
{
	auto& input = Input::GetInstance();
	//P�{�^���Ń|�[�Y����
	if (input.IsTrigger("Pause")) 
	{
		m_update = &PauseScene::DisappearUpdate;
		m_draw = &PauseScene::ShiftingDraw;
		m_countFrame = kAppearInterval;
		return;
	}
}

void PauseScene::DisappearUpdate()
{
	--m_countFrame;
	if (m_countFrame < 0) 
	{
		m_controller.PopScene();
		return;
	}
}

void PauseScene::NormalDraw()
{
	float wSize = Game::kScreenWidth;
	float hSize = Game::kScreenHeight;
	//�w�i�𔒂��ۂ�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(10, 10,//����
		wSize - 10, hSize - 10,//�E��
		0xffffff,//�J���[
		true);//�h��ׂ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�|�[�Y��ʘg
	DrawBoxAA(10.0f, 10.0f,//����
		wSize - 10.0f, hSize - 10.0f,//�E��
		0xffffff,//�J���[
		false,//�h��ׂ��Ȃ�
		3.0f);//������Ƒ��ڂ̐�

	DrawString(30, 30, " Pause ", 0xffffff);
	DrawString(wSize / 3.0f, hSize / 2.0f, " ���チ�j���[�������\��ł� ", 0xff0000);
}

void PauseScene::ShiftingDraw()
{
	float wSize = Game::kScreenWidth;
	float hSize = Game::kScreenHeight;

	//���̍������v�Z����
	const int frameHeight = hSize - 10 - 10;
	//�^�񒆂�Y���W���v�Z����
	const int middleY = (10 + hSize - 10) / 2;//�^�񒆂�Y���W

	//�����v�Z
	float rate = static_cast<float>(m_countFrame) / static_cast<float>(kAppearInterval);

	//���݂̎��Ԃł��蓾��ׂ������̔������v�Z���܂�
	int halfHeight = static_cast<int>((frameHeight * rate) * 0.5f);

	//��������Atop(��)��bottom(��)���v�Z���܂�
	int top = middleY - halfHeight;
	int bottom = middleY + halfHeight;
	//�w�i�𔒂��ۂ�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
	DrawBox(10, top,//����
		wSize - 10, bottom,//�E��
		0xffffff,//�J���[
		true);//�h��ׂ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�|�[�Y��ʘg
	DrawBoxAA(10.0f, static_cast<float>(top),//����
		wSize - 10.0f, static_cast<float>(bottom),//�E��
		0xffffff,//�J���[
		false,//�h��ׂ��Ȃ�
		3.0f);//������Ƒ��ڂ̐�
}
