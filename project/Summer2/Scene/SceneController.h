#pragma once
//�X�}�[�g�|�C���^���g��
#include<memory>
#include<list>
class SceneBase;
class Input;
/// <summary>
/// �e�V�[�����Ǘ�����N���X
/// �Ǘ��͂��邪�A�@�ۂ��Ǘ��͂��Ȃ�(�@�ۂ̓N���X�ǂ������)
/// �C���^�[�t�F�C�X��񋟂���N���X
/// </summary>
class SceneController
{
public:
	SceneController();
	/// <summary>
	/// Application����Ă΂��֐��B���݂̃V�[����Update�����̂܂܎��s�B
	/// </summary>
	/// <param name="input"></param>
	void Update(Input& input);
	/// <summary>
	/// Application����Ă΂��֐��B���݂̃V�[����Draw�����̂܂܎��s�B
	/// </summary>
	/// <param name="input"></param>
	void Draw();
	/// <summary>
	/// ���̏�Ԃ��Z�b�g����(�Z�b�g����̂͊e��Ԃ̖���)
	/// �e�V�[�����R���g���[���[�����Đ؂�ւ�������
	/// </summary>
	/// <param name="">���̏�Ԃ̃|�C���^</param>
	void ChangeScene(std::shared_ptr<SceneBase> scene);
	/// <summary>
	/// �ŏ��ɓ��ꂽ�V�[����؂�ւ���
	/// </summary>
	/// <param name=""></param>
	void ChangeBaseScene(std::shared_ptr<SceneBase> scene);
	/// <summary>
	/// �V�[�����v�b�V��(�����)����
	/// </summary>
	/// <param name=""></param>
	void PushScene(std::shared_ptr<SceneBase> scene);
	/// <summary>
	/// �V�[�����|�b�v(���o��)����
	/// </summary>
	/// <param name=""></param>
	void PopScene(std::shared_ptr<SceneBase> scene);

private:
	using SceneStack_t = std::list<std::shared_ptr<SceneBase>>;//�V�[�����X�^�b�N�\���ɂ��邽�߂̌^
	SceneStack_t m_scenes;//���ݎ��s���̃V�[��(���������͒m��Ȃ�)
};

