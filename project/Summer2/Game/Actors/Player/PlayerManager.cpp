#include "PlayerManager.h"
#include "Player.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../Game/Actors/Actor.h"
#include "../../../General/Collidable.h"
#include "../../../General/Collision/CollisionChecker.h"
#include "../../../General/Collision/SearchTrigger.h"
#include "../../../General/Math/MyMath.h"

PlayerManager::PlayerManager(std::shared_ptr<Player> player):
	m_player(player),
	m_triggerChecker(std::make_shared<CollisionChecker>())
{
}

PlayerManager::~PlayerManager()
{
}

void PlayerManager::Update(std::vector<std::shared_ptr<Actor>> actors)
{
	//���G
	SearchTarget(actors);
}

void PlayerManager::SearchTarget(std::vector<std::shared_ptr<Actor>> actors)
{
	//�v���C���[�̃g���K�[
	auto triggerP = m_player->GetSearchTrigger();
	//������
	triggerP->Init();
	//�ŒZ����
	float shortDis = 0.0f;
	//�^�[�Q�b�g
	Vector3 target = Vector3::Zero();
	for (auto& actor : actors)
	{
		//�`��p�̃N���X�͖���
		if (actor->IsDrawOnly())continue;
		if (actor->GetCollidable()->GetGameTag() == GameTag::Enemy)
		{
			//�q�b�g������
			bool isHit = false;
		
			//�R���C�_�u��
			auto collP = triggerP->GetCollidable();
			//�G�̃R���C�_�u��
			auto collE = actor->GetCollidable();
			//���Ȃ�
			if (collE->GetColl()->GetShape() == Shape::Sphere)
			{
				//�͈͓��ɓG�����邩�`�F�b�N
				if (m_triggerChecker->CheckCollSS(collP, collE))
				{
					isHit = true;
				}
			}
			//�J�v�Z��
			else if (collE->GetColl()->GetShape() == Shape::Capsule)
			{
				//�͈͓��ɓG�����邩�`�F�b�N
				if (m_triggerChecker->CheckCollCS(collE, collP))
				{
					isHit = true;
				}
			}
			//�q�b�g�����Ȃ王��p���ɂ��邩���`�F�b�N
			if (!isHit)continue;
			//�O�x�N�g���Ƒ��΃x�N�g���Ŋp�x���m�F
			auto forward = triggerP->GetViewForward();
			auto pToE = collE->GetRb()->GetNextPos() - collP->GetRb()->GetPos();
			float dis = pToE.Magnitude();
			//�p�x�v�Z
			auto angle = Vector2::Theata(forward.XZ(), pToE.XZ());
			//����p���ɂ����Ȃ�
			if (abs(angle) > triggerP->GetViewAngle())continue;
			//�ŒZ�����Ȃ�
			if (shortDis == 0.0f || shortDis > dis)
			{
				shortDis = dis;
				//�^�[�Q�b�g
				target = collE->GetRb()->GetNextPos();
				//�^�[�Q�b�g�̍��W��ۑ�
				triggerP->OnHitTarget(target);
			}
		}
	}
}
