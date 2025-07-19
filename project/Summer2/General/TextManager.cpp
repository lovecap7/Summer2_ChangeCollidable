#include "TextManager.h"
#include <DxLib.h>
#include <cassert>

//������
void TextManager::Init()
{
	m_handles["���C���I"] = CreateFontToHandle("Meiryo�i���C���I�j", 32, 5, DX_FONTTYPE_ANTIALIASING);
	//���[�h�ɐ����������`�F�b�N
	for (auto& [key, value] : m_handles) {
		assert(value >= 0);
	}
}

//�X�V
void TextManager::Update()
{

}
//�`��
void TextManager::Draw()const
{
	DrawStringToHandle(100, 500, "���m�x���̃Z���t�\��", 0xffffff, m_handles["���C���I"]);
}
//�I��
void TextManager::End()
{
	//���[�h�ɐ����������`�F�b�N
	for (auto& [key, value] : m_handles) {
		DeleteFontToHandle(value);
	}
	m_handles.clear();
}