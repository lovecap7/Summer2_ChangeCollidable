#pragma once
#include <memory>
#include <list>
class MyEffect;
/// <summary>
/// �G�t�F�N�g���Ǘ�����V���O���g���N���X
/// </summary>
class EffekseerManager final
{
private:
	//�V���O���g���̏���
	EffekseerManager() = default;
	~EffekseerManager() = default;
	//�R�s�[�֎~
	EffekseerManager(const EffekseerManager&) = delete;
	EffekseerManager& operator = (const EffekseerManager&) = delete;
	//���[�u�֎~
	EffekseerManager(EffekseerManager&&) = delete;
	EffekseerManager& operator = (EffekseerManager&&) = delete;
public:
	//�C���X�^���X���擾
	static EffekseerManager& GetInstance()
	{
		static EffekseerManager instance;
		return instance;
	}
	//�o�^
	void Entry(std::shared_ptr<MyEffect> eff);
	//����
	void Exit(std::shared_ptr<MyEffect> eff);
	//������
	void Init();
	//�X�V����
	void Update();
	//�`��
	void Draw()const;
	//�I������
	void End();
private:
	//�G�t�F�N�g�̔z��
	std::list<std::shared_ptr<MyEffect>> m_effects;
};

