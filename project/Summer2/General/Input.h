#pragma once
#include <map>
#include <string>
#include <vector>
#include <list>

class Input
{
public:
	/// <summary>
	/// �X�e�B�b�N�̌X����ێ�����\����
	/// </summary>
	struct StickInfo
	{
		/// <summary>
		/// ��(Max -1000)
		/// �E(Max  1000)
		/// </summary>
		int leftStickX;
		/// <summary>
		/// ��(Max -1000)
		/// ��(Max  1000)
		/// </summary>
		int leftStickY;
		/// <summary>
		/// ��(Max -1000)
		/// �E(Max  1000)
		/// </summary>
		int rightStickX;
		/// <summary>
		/// ��(Max -1000)
		/// ��(Max  1000)
		/// </summary>
		int rightStickY;
		StickInfo()
		{
			leftStickX = 0;
			leftStickY = 0;
			rightStickX = 0;
			rightStickY = 0;
		}
		//���͂����������ǂ������擾(�ړ���Ԃɐ؂�ւ���Ƃ��Ɏg��)
		bool IsLeftStickInput() { return leftStickX != 0 || leftStickY != 0; };
		bool IsRightStickInput() { return rightStickX != 0 || rightStickY != 0; };
	};

	struct TriggerInfo
	{
		TriggerInfo()
		{
			left = 0;
			right = 0;
		}
		int left;
		int right;
	};
	~Input();
	/// <summary>
	/// �L�[�̏����X�V����
	/// </summary>
	void Init();

	/// <summary>
	/// ���͏����X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// ���͏����󂯕t���Ȃ��悤�ɂ���
	/// </summary>
	void StopUpdate();

	/// <summary>
	/// �L�[���������ǂ���
	/// </summary>
	/// <param name="keyName">���肵�����L�[</param>
	// <returns>���������ǂ���</returns>
	bool IsPress(const std::string& action)const;

	/// <summary>
	/// �L�[�������ꂽ�u�Ԃ��擾����
	/// </summary>
	/// <param name="keyName">���肵�����L�[</param>
	/// <returns>�����ꂽ�u�Ԃ̂�true</returns>
	bool IsTrigger(const std::string& action)const;

	/// <summary>
	/// �L�[�𗣂����u�Ԃ��擾����
	/// </summary>
	/// <param name="keyName">���肵�����L�[</param>
	/// <returns>�����ꂽ�u�Ԃ̂�true</returns>
	bool IsRelease(const std::string& action)const;

	/// <summary>
	/// �X�e�B�b�N�̌X�����擾����
	/// </summary>
	/// <returns>�X�e�B�b�N���ǂꂾ���|��Ă��邩</returns>
	StickInfo GetStickInfo()const { return m_stickInfo; }

	/// <summary>
	/// ���������|���Ă���Ȃ�true
	/// </summary>
	/// <returns></returns>
	bool IsLowPowerLeftStick()const;
	/// <summary>
	/// �^�񒆂��炢�œ|������true
	/// </summary>
	/// <returns></returns>
	bool IsMediumPowerLeftStick()const;
	/// <summary>
	/// �ő�܂œ|������
	/// </summary>
	/// <returns></returns>
	bool IsHighPowerLeftStick()const;

	/// <summary>
	/// �g���K�[�̉������݋���擾����
	/// </summary>
	/// <returns>�g���K�[���ǂ̂��炢�������܂�Ă��邩</returns>
	TriggerInfo GetTriggerInfo() { return m_triggerInfo; }

	/// <summary>
	/// �g���K�[�{�^���������Ă��邩�ǂ�����Ԃ�(�������݂̋�����)
	/// </summary>
	/// <param name="right">�E���̃{�^�����擾�������Ȃ�true</param>
	/// <param name="power">�ǂ̂��炢�������܂�Ă�����true�Ƃ��邩(�ő�255,�ŏ�0)</param>
	/// <returns></returns>
	bool IsPushTrigger(bool right, int power);

	/// <summary>
	/// �g���K�[�{�^���������Ă��邩�ǂ�����Ԃ�(�������݂̋����Œ�)
	/// </summary>
	/// <param name="right"></param>
	/// <returns></returns>
	bool IsPushTrigger(bool right);

private:
	/// <summary>
	/// ���͂̎��
	/// </summary>
	enum class InputType
	{
		kKeyboard,
		kPad,
		kMouse
	};
	struct InputMapInfo
	{
		InputType type;
		int buttonID;
	};

	std::map<std::string, std::vector<InputMapInfo>> m_inputActionMap;

	//���݂̃t���[���̓���
	std::map<std::string, bool>m_currentInput;
	//1�t���[���O�̓���
	std::map<std::string, bool>m_lastInput;
	//�X�e�B�b�N�Ɋւ�����͏��
	StickInfo m_stickInfo = StickInfo();
	//�g���K�[�̏��
	TriggerInfo m_triggerInfo = TriggerInfo();
	
};


