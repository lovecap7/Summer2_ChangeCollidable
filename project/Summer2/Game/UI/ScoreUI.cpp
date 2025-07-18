#include "ScoreUI.h"
#include "../../General/game.h"
#include "../Actors/ActorManager.h"
#include "../GameRule/Score.h"
#include <DxLib.h>

namespace
{
	//�傫��
	constexpr float kScale = 0.3f;
	//��
	constexpr int kImageWidth = 256;
	constexpr int kImageHeight = 256;
	//�X�R�A��X���W
	constexpr int kPosX = (Game::kScreenWidth - 100);
	//�X�R�A��Y���W
	constexpr int kPosY = 50;
	//�X�R�A��1���̕�
	constexpr int kDigitMargin = 50;
	//�d��
	constexpr float kGravity = 0.98f;
	//�W�����v��
	constexpr float kJumpPower = 2.0f;
}

ScoreUI::ScoreUI(int handle, const std::weak_ptr<Score> score) :
	UIBase(UIData::Score,handle),
	m_viewScore(0),
	m_viewMaxScore(0),
	m_score(score),
	m_digits{}
{
	for (auto& pos : m_viewPoses)
	{
		pos = { kPosX ,kPosY };
	}
}

ScoreUI::~ScoreUI()
{
}

void ScoreUI::Init()
{
}

void ScoreUI::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//�v���C���[�܂��̓{�X���������ꍇ�͂���UI���폜
	if (actorManager.lock()->GetPlayer().expired() || actorManager.lock()->GetBoss().expired())
	{
		m_isDelete = true;
		return;
	}
	//�X�R�A�X�V
	m_viewMaxScore = m_score.lock()->GetScore();
	//���݂̃X�R�A���܂��ő�܂ŒB���Ă��Ȃ��Ȃ�
	if (m_viewScore != m_viewMaxScore)
	{
		auto score = MathSub::Lerp(static_cast<float>(m_viewScore), static_cast<float>(m_viewMaxScore), 0.5f);
		if (score + 1.0f >= static_cast<float>(m_viewMaxScore))
		{
			score = m_viewMaxScore;
		}
		m_viewScore = score;
	}
	//���o���l
	int score = m_viewScore;
	for (int i = 0;i < kDigitNum;++i)
	{
		//���݂̒l
		auto digit = m_digits[i];
		//���o���ĕۑ�
		m_digits[i] = score % 10;
		//����������
		score = score / 10;
		//�����ς���Ă�Ȃ�
		if (digit != m_digits[i])
		{
			//�����͂˂�
			m_viewPoses[i].y -= kJumpPower;
		}
		//����ȏ�Y���W��������Ȃ��悤�ɕ␳
		m_viewPoses[i].y += kGravity;
		if (m_viewPoses[i].y > kPosY)
		{
			m_viewPoses[i].y = kPosY;
		}
	}
}

void ScoreUI::Draw() const
{
	for (int i = 0;i < kDigitNum;++i)
	{
		//�؂�����v�Z����
		int sizeX, sizeY;
		GetGraphSize(m_handle, &sizeX, &sizeY);//�摜�T�C�Y
		int cutX = m_digits[i] % (sizeX / kImageWidth);//��
		int cutY = m_digits[i] / (sizeX / kImageWidth);//�c
		//�`��
		DrawRectRotaGraphFast(m_viewPoses[i].x - i * kDigitMargin, m_viewPoses[i].y,
			kImageWidth * cutX,
			kImageHeight * cutY,
			kImageWidth, kImageHeight,
			kScale, 0.0f, m_handle, true, false);
	}
}

void ScoreUI::End()
{
}
