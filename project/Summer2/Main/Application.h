#pragma once

/// <summary>
/// �V���O���g��
/// </summary>
class Application
{
private:
	//�V���O���g���̏���
	Application() = default;
	~Application() = default;
	//�R�s�[�֎~
	Application(const Application&) = delete;
	Application& operator = (const Application&) = delete;
	//���[�u�֎~
	Application(Application&&) = delete;
	Application& operator = (Application&&) = delete;
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
	/// <summary>
	/// �X�V����
	/// </summary>
	void Run();
	/// <summary>
	/// �I������
	/// </summary>
	void Terminate();
};
