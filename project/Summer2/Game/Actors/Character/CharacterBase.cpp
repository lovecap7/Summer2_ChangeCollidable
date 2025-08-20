#include "CharacterBase.h"
#include "../../../General/Model.h"
#include "../../../General/Sound/SoundManager.h"

CharacterBase::CharacterBase(Shape shape) :
	Actor(shape)
{
}

std::weak_ptr<SE>  CharacterBase::CharacterOnceSE(std::string name)
{
	return SoundManager::GetInstance().PlayOnceSE(m_soundHandles.at(name));
}

std::weak_ptr<SE>  CharacterBase::CharacterLoopSE(std::string name)
{
	return SoundManager::GetInstance().PlayLoopSE(m_soundHandles.at(name));
}

std::weak_ptr<Voice> CharacterBase::CharacterVC(std::string name)
{
	return SoundManager::GetInstance().PlayVC(m_soundHandles.at(name));
}

void CharacterBase::EndSound()
{
	//íœ
	for (const auto& [key, value] : m_soundHandles) {
		DeleteSoundMem(value);
	}
}