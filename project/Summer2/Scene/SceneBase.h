#pragma once
class Input;
class SceneController;

/// <summary>
/// �e�V�[���̊��N���X
/// </summary>
class SceneBase
{
protected:
	//�e�V�[���̏�Ԃ�؂�ւ��邽��
	SceneController& m_controller;
public:
	SceneBase(SceneController& controller);
	/// <summary>
	/// ����������
	/// </summary>
	virtual void Init() abstract;
	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	/// <param name="input"></param>
	virtual void Update(Input& input) abstract;
	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() abstract;
	/// <summary>
	/// �I������
	/// </summary>
	virtual void End() abstract;
};

