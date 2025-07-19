#pragma once
#include <map>
#include <string>
/// <summary>
/// �e�L�X�g�f�[�^���Ǘ�����V���O���g���N���X
/// </summary>
class TextManager final
{
private:
	//�V���O���g���̏���
	TextManager() = default;
	~TextManager() = default;
	//�R�s�[�֎~
	TextManager(const TextManager&) = delete;
	TextManager& operator = (const TextManager&) = delete;
	//���[�u�֎~
	TextManager(TextManager&&) = delete;
	TextManager& operator = (TextManager&&) = delete;
public:
	//�C���X�^���X���擾
	static TextManager& GetInstance()
	{
		static TextManager instance;
		return instance;
	}
	//������
	void Init();
	//�X�V
	void Update();
	//�`��
	void Draw()const;
	//�I������
	void End();
private:
	//�n���h��
	std::map<std::string, int> m_handles;
};

