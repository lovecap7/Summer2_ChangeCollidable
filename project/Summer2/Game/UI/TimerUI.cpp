#include "TimerUI.h"
#include "../GameRule/Timer.h"
#include "../../General/game.h"
#include "../Actors/ActorManager.h"
#include <DxLib.h>

namespace
{
	//�X�R�A�̌���
	constexpr int kDigitNum = 2;
	//�傫��
	constexpr float kScale = 0.2f;
	//��
	constexpr int kImageWidth = 256;
	constexpr int kImageHeight = 256;
	//�^�C�}�[��X���W
	constexpr int kMinSecPosX = (Game::kScreenWidth - 100);
	constexpr int kSecPosX = (Game::kScreenWidth - 170);
	constexpr int kMinPosX = (Game::kScreenWidth - 240);
	//�^�C�}�[��Y���W
	constexpr int kPosY = 120;
	//�^�C�}�[��1���̕�
	constexpr int kDigitMargin = 30;
}

TimerUI::TimerUI(int handle,const std::weak_ptr<Timer> timer):
	UIBase(UIData::Timer,handle),
	m_timer(timer)
{
}

TimerUI::~TimerUI()
{
}

void TimerUI::Init()
{
}

void TimerUI::Update(const std::weak_ptr<ActorManager> actorManager)
{
	if (m_timer.expired() || actorManager.lock()->GetBoss().expired())
	{
		m_isDelete = true;
		return;
	}
}

void TimerUI::Draw() const
{
	if (m_timer.expired())return;
	auto time = m_timer.lock();
	int minSec = time->GetMillisecond();
	int sec = time->GetSeconds();
	int min = time->GetMinutes();
	//�e��
	int digits[kDigitNum];
	for (int i = 0;i < kDigitNum;++i)
	{
		//���o���ĕۑ�
		digits[i] = minSec % 10;
		//����������
		minSec = minSec / 10;
		//�؂�����v�Z����
		int sizeX, sizeY;
		GetGraphSize(m_handle, &sizeX, &sizeY);//�摜�T�C�Y
		int cutX = digits[i] % (sizeX / kImageWidth);//��
		int cutY = digits[i] / (sizeX / kImageWidth);//�c
		//�`��
		DrawRectRotaGraphFast(kMinSecPosX - i * kDigitMargin, kPosY,
			kImageWidth * cutX,
			kImageHeight * cutY,
			kImageWidth, kImageHeight,
			kScale, 0.0f, m_handle, true, false);
	}
	for (int i = 0;i < kDigitNum;++i)
	{
		//���o���ĕۑ�
		digits[i] = sec % 10;
		//����������
		sec = sec / 10;
		//�؂�����v�Z����
		int sizeX, sizeY;
		GetGraphSize(m_handle, &sizeX, &sizeY);//�摜�T�C�Y
		int cutX = digits[i] % (sizeX / kImageWidth);//��
		int cutY = digits[i] / (sizeX / kImageWidth);//�c
		//�`��
		DrawRectRotaGraphFast(kSecPosX - i * kDigitMargin, kPosY,
			kImageWidth * cutX,
			kImageHeight * cutY,
			kImageWidth, kImageHeight,
			kScale, 0.0f, m_handle, true, false);
	}
	for (int i = 0;i < kDigitNum;++i)
	{
		//���o���ĕۑ�
		digits[i] = min % 10;
		//����������
		min = min / 10;
		//�؂�����v�Z����
		int sizeX, sizeY;
		GetGraphSize(m_handle, &sizeX, &sizeY);//�摜�T�C�Y
		int cutX = digits[i] % (sizeX / kImageWidth);//��
		int cutY = digits[i] / (sizeX / kImageWidth);//�c
		//�`��
		DrawRectRotaGraphFast(kMinPosX - i * kDigitMargin, kPosY,
			kImageWidth * cutX,
			kImageHeight * cutY,
			kImageWidth, kImageHeight,
			kScale, 0.0f, m_handle, true, false);
	}
}

void TimerUI::End()
{
}
