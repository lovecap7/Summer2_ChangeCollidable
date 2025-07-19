#include "Application.h"
#include <DxLib.h>
#include "../General/game.h"
#include "../Scene/SceneController.h"
#include  "../General/Input.h"
#include  "../General/Collision/Physics.h"
#include  "../General/Effect/EffekseerManager.h"
#include  "../General/TextManager.h"

Application& Application::GetInstance()
{
    //���̎��_�Ń��������m�ۂ���ăA�v���I���܂Ŏc��
    static Application app;
    return app;
}

bool Application::Init()
{
	//�Q�[���^�C�g��
	SetWindowText("Spinning Knight");
	//�Q�[���A�C�R��
	//SetWindowIconID(IDI_ICON1);

	 //�t���X�N���[���łȂ��A�E�B���h�E���[�h�ŊJ���悤�ɂ���
	//�����������֐��̓E�B���h�E���J���O��(Dxlib.Init()�̑O)�ɏ������Ă����K�v������
	ChangeWindowMode(Game::kDefaultWindowMode);

	// DirectX11���g�p����悤�ɂ���B(DirectX9���A�ꕔ�@�\�s��)
	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	//��ʃT�C�Y�ύX
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return false;			// �G���[���N�����璼���ɏI��
	}

	//�`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	//3D�`��̂��߂̏���
	//Z�o�b�t�@���g�p����
	SetUseZBuffer3D(true);
	//Z�o�b�t�@�ɏ������݂��s��
	SetWriteZBuffer3D(true);
	//�o�b�N�J�����O���s��(�|���S���̗��ʂ�`�悵�Ȃ��悤�ɂ���)
	SetUseBackCulling(true);

	//Physics�̏�����
	Physics::GetInstance().Init();
	//�G�t�F�N�g�̏�����
	EffekseerManager::GetInstance().Init();

    return true;
}

void Application::Run()
{
	//�A�v���P�[�V�����ȊO�͂����Ő錾�Ə�����
	SceneController* sceneController = new SceneController();
	//�R���g���[���[
	Input::GetInstance().Init();
	//Physics(�Փˏ���)
	auto& physics = Physics::GetInstance();
	//�G�t�F�N�g
	auto& effect = EffekseerManager::GetInstance();
	//�e�L�X�g�}�l�[�W���[
	auto& text = TextManager::GetInstance();
	text.Init();

	//�Q�[�����[�v
	while (ProcessMessage() != -1) // Windows���s��������҂�
	{
		//����̃��[�v���n�܂������Ԃ��o���Ă���
		LONGLONG time = GetNowHiPerformanceCount();

		//��ʑS�̂��N���A
		ClearDrawScreen();

		//�����ɃQ�[���̏���������
		
		//�X�V
		Input::GetInstance().Update();
		sceneController->Update();
		physics.Update();
		effect.Update();
		text.Update();
		//�`��
		sceneController->Draw();
		effect.Draw();
		text.Draw();

#if _DEBUG
		DrawFormatString(0, 500, 0xff0000, "FPS : %.2f", GetFPS());
#endif

		//��ʂ̐؂�ւ���҂K�v������
		ScreenFlip();//1/60�b�o�߂���܂ő҂�

		//FPS��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667);
		{

		}

		//ESC�L�[�ŏI��
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			sceneController = nullptr;
			break;
		}
	}
	delete sceneController;
	sceneController = nullptr;
}

void Application::Terminate()
{
	Physics::GetInstance().Reset();
	EffekseerManager::GetInstance().End();
	TextManager::GetInstance().End();
	DxLib_End();				// �c�w���C�u�����g�p�̏I������
}

