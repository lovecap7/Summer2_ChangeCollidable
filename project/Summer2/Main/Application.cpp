#include "Application.h"
#include <DxLib.h>
#include "../General/game.h"
#include "../Scene/SceneController.h"
#include  "../General/Input.h"
#include  "../General/Collision/Physics.h"

Application& Application::GetInstance()
{
    //���̎��_�Ń��������m�ۂ���ăA�v���I���܂Ŏc��
    static Application app;
    return app;
}

bool Application::Init()
{
  
	//�Q�[���^�C�g��
	SetWindowText("�^�C�g��");
	//�Q�[���A�C�R��
	//SetWindowIconID(IDI_ICON1);

	 //�t���X�N���[���łȂ��A�E�B���h�E���[�h�ŊJ���悤�ɂ���
	//�����������֐��̓E�B���h�E���J���O��(Dxlib.Init()�̑O)�ɏ������Ă����K�v������
	ChangeWindowMode(Game::kDefaultWindowMode);
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

    return true;
}

void Application::Run()
{
	//�A�v���P�[�V�����ȊO�͂����Ő錾�Ə�����
	SceneController* sceneController = new SceneController();
	//�R���g���[���[
	Input::GetInstance().Init();

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
		Physics::GetInstance().Update();
		//�`��
		sceneController->Draw();

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
	DxLib_End();				// �c�w���C�u�����g�p�̏I������
}

