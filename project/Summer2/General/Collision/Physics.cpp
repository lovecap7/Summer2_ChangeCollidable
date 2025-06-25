#include "Physics.h"
#include "CollisionChecker.h"
#include "CollisionProcess.h"
#include "../Rigidbody.h"
#include "CollisionProcess.h"
#include "../game.h"
#include <cassert>

namespace
{
	//�m�F��
	constexpr int kTryNum = 30;
}

void Physics::Init()
{
	m_collChecker = std::make_shared<CollisionChecker>();
	m_collProcessor = std::make_shared<CollisionProcess>();
}

void Physics::Entry(std::shared_ptr<Collidable> collidable)
{
	// �o�^
	bool found = (std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end());
	if (!found)
	{
		m_collidables.emplace_back(collidable);
	}
	// ���ɓo�^����Ă���G���[
	else
	{
		assert(0 && "�w���collidable�͓o�^�ςł��B");
	}
}

void Physics::Exit(std::shared_ptr<Collidable> collidable)
{
	// �o�^����
	bool found = (std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end());
	if (found)
	{
		m_collidables.remove(collidable);
	}
	// �o�^����ĂȂ�������G���[
	else
	{
		assert(0 && "�w���collidable���o�^����Ă��܂���B");
	}
}

void Physics::Update()
{
	//�d��
	Gravity();
	//���ƕǂ̂Ƃ̓��������t���O��������
	for (auto& collidable : m_collidables)
	{
		collidable->ResetHitFlag();
	}

	//�x�������p
	std::list<OnCollideInfo> onCollideInfo;
	//��x���������Ă��Ȃ��̂Ȃ烋�[�v���I������
	bool isOneMore = false;
	//�␳�������Ƃŕʂ̃I�u�W�F�N�g�ɓ�����\��������̂ň��񐔃`���b�N����
	for (int i = 0;i < kTryNum;++i)
	{
		isOneMore = false;
		//�����蔻����`�F�b�N
		for (auto& collA : m_collidables)
		{
			//�����蔻����s��Ȃ��Ȃ��΂�
			if (collA->GetGameTag() == GameTag::None)continue;
			if (collA->m_isThrough)continue;

			for (auto& collB : m_collidables)
			{
				//�����Ƃ͓����蔻������Ȃ�
				if (collA == collB)continue;
				//�����蔻����s��Ȃ��Ȃ��΂�
				if (collB->GetGameTag() == GameTag::None)continue;
				if (collB->m_isThrough)continue;

				//�������Ă�Ȃ�
				if (m_collChecker->IsCollide(collA, collB))
				{
					//�ǂ�����g���K�[�Ȃł͂Ȃ��Ȃ�
					if (!collA->m_isTrigger && !collB->m_isTrigger)
					{
						//�Փˏ���
						m_collProcessor->FixNextPos(collA, collB);
						//�ύX��̈ʒu�ōēx�ق��̃R���C�_�u���ɓ�����\��������̂�
						//������x�`�F�b�N����K�v������
						isOneMore = true;
					}

					//����܂łɓ���������񂪂��邩���`�F�b�N
					bool isCollAInfo = false;
					bool isCollBInfo = false;
					for (const auto& item : onCollideInfo)
					{
						// ���ɒʒm���X�g�Ɋ܂܂�Ă�����Ă΂Ȃ�
						if (item.owner == collA)
						{
							isCollAInfo = true;
						}
						if (item.owner == collB)
						{
							isCollBInfo = true;
						}
					}
					if (!isCollAInfo)
					{
						onCollideInfo.emplace_back(OnCollideInfo{ collA, collB });
					}
					if (!isCollBInfo)
					{
						onCollideInfo.emplace_back(OnCollideInfo{ collB, collA });
					}
				}
			}
		}

		//�`�F�b�N�̕K�v���Ȃ��Ȃ�
		if (!isOneMore)break;
	}

	//�ʒu�m��
	for (auto& coll : m_collidables)
	{
		//�ʒu���m��
		coll->Complete();
	}

	// ������ʒm
	for (auto& collInfo : onCollideInfo)
	{
		collInfo.OnCollide();
	}
}

Physics::~Physics()
{
}

void Physics::Gravity()
{
	for (auto& collidable : m_collidables)
	{
		//�d�͂��󂯂邩
		if (!collidable->m_rb->m_isGravity)continue;
		auto rb = collidable->m_rb;
		//�d�͂�������
		rb->m_vec += Gravity::kGravity;
		//�d�͂ɏ��������
		if (rb->m_vec.y < Gravity::kMaxGravityY)
		{
			//����𒴂��Ȃ��悤�ɂ���
			rb->m_vec.y = Gravity::kMaxGravityY;
		}
	}
}