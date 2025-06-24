#include "CollisionManager.h"
#include "../../Game/Actors/Actor.h"
#include "CollisionChecker.h"
#include "CollisionProcess.h"
#include "../Rigidbody.h"
#include "ColliderBase.h"
#include "../Collidable.h"

namespace
{
	//�m�F��
	constexpr int kTryNum = 30;
}

CollisionManager::CollisionManager():
	m_collChecker(std::make_shared<CollisionChecker>()),
	m_collProcessor(std::make_shared<CollisionProcess>())
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Update(std::vector<std::shared_ptr<Actor>> actors)
{
	//��x���������Ă��Ȃ��̂Ȃ烋�[�v���I������
	bool isOneMore = false;
	//�␳�������Ƃŕʂ̃I�u�W�F�N�g�ɓ�����\��������̂ň��񐔃`���b�N����
	for (int i = 0;i < kTryNum;++i)
	{
		isOneMore = false;
		//�����蔻����`�F�b�N
		for (auto& actorA : actors)
		{
			//�`��p�̃N���X�͖���
			if (actorA->IsDrawOnly())continue;
			auto collA = actorA->GetCollidable();

			//�����蔻����s��Ȃ��Ȃ��΂�
			if (collA->GetGameTag() == GameTag::None)continue;
			if (!collA->IsCollide())continue;

			for (auto& actorB : actors)
			{
				//�`��p�̃N���X�͖���
				if (actorB->IsDrawOnly())continue;
				auto collB = actorB->GetCollidable();

				//�����蔻����s��Ȃ��Ȃ��΂�
				if (collB->GetGameTag() == GameTag::None)continue;
				if (!collB->IsCollide())continue;

				//�����Ƃ͓����蔻������Ȃ�
				if (actorA == actorB)continue;

				//�������Ă邩���`�F�b�N
				bool isHit = false;

				//����
				if (collA->GetColl()->GetShape() == Shape::Sphere)
				{
					//��
					if (collB->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollSS(collA, collB);
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessSS(collA, collB);
						}
					}
					//�J�v�Z��
					else if (collB->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCS(collB, collA);
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessCS(collB, collA);
						}
					}
					//�|���S��
					else if (collB->GetColl()->GetShape() == Shape::Polygon)
					{
						isHit = m_collChecker->CheckCollSP(collA, collB);
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessSP(collA, collB);
						}
					}
				}
				//�J�v�Z����
				else if (collA->GetColl()->GetShape() == Shape::Capsule)
				{
					//��
					if (collB->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollCS(collA, collB);
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessCS(collA, collB);
						}
					}
					//�J�v�Z��
					else if (collB->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCC(collA, collB);
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessCC(collA, collB);
						}
					}
					//�|���S��
					else if (collB->GetColl()->GetShape() == Shape::Polygon)
					{
						isHit = m_collChecker->CheckCollCP(collA, collB);
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessCP(collA, collB);
						}
					}
				}
				//�|���S����
				else if (collA->GetColl()->GetShape() == Shape::Polygon)
				{
					//��
					if (collB->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollSP(collB, collA);
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessSP(collB, collA);
						}
					}
					//�J�v�Z��
					else if (collB->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCP(collB, collA);
						if (isHit)
						{
							//�x�N�g����␳����
							m_collProcessor->ProcessCP(collB, collA);
						}
					}
				}

				//�������Ă�Ȃ�
				if (isHit)
				{
					//�����������̏���
					actorA->OnHitColl(collB);
					actorB->OnHitColl(collA);
					isOneMore = true;
				}
			}
		}
		
		//�`�F�b�N�̕K�v���Ȃ��Ȃ�
		if (!isOneMore)break;
	}
}
