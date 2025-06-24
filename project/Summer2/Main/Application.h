#pragma once

/// <summary>
/// �V���O���g��
/// </summary>
class Application
{
public:
	/// <summary>
	/// �V���O���g���I�u�W�F�N�g��Ԃ��֐�
	/// �Q�Ƃ�Ԃ����ƂŁA������h��
	/// �ԈႦ�Ēl��Ԃ����ꍇ�A�R�s�[�R���X�g���N�^������̂�
	/// �����private�ɂ��Ă������ƂŒe�����Ƃ��ł���
	/// </summary>
	/// <returns></returns>
	static Application& GetInstance();

	/// <summary>
	/// �A�v���P�[�V�����̏�����
	/// </summary>
	/// <returns>ture:����������, false:���s</returns>
	bool Init();

	void Run();

	void Terminate();
private:
	Application() {}
	//�֐��錾�̌�� = delete�ł��̊֐��̎g�p���֎~�ł���
	Application(const Application&) {}
	void operator=(const Application&) {}
};

//const
//�l�̕ύX���ł��Ȃ�
//�֐��̌��ɂ��Ă���ꍇ��const�̎�����̏�Ԃ��ύX�s��

//static
//�A�h���X���m�肳����
//�ǂ�����ł��A�N�Z�X�ł���(�O���[�o��)
//���[�J���ϐ���static�̏ꍇ�͂��̃��[�J���ϐ��̎����債���A�N�Z�X�ł��Ȃ�
//�V���O���g���I�u�W�F�N�g�͂���������ł��邱�Ƃ�ۏ؂���Ă��邽�߃A�N�Z�X�ɂ����
//�ʂ̃I�u�W�F�N�g�����Ă���Ƃ����悤�Ȃ��Ƃ͋N���Ȃ�
