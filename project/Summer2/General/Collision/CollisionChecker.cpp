#include "CollisionChecker.h"
#include "../Math/MyMath.h"
#include <DxLib.h>
#include "Collidable.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PolygonCollider.h"
#include "../Rigidbody.h"

CollisionChecker::CollisionChecker()
{
}


bool CollisionChecker::IsCollide(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//�R���C�_�[�̌`��
	auto shapeA = collA->m_collisionData->m_shape;
	auto shapeB = collB->m_collisionData->m_shape;

	//�����������̌���
	bool isHit = false;
	//����
	if (shapeA == Shape::Sphere)
	{
		//��
		if (shapeB == Shape::Sphere)
		{
			isHit = CheckCollSS(collA, collB);
		}
		//�J�v�Z��
		else if (shapeB == Shape::Capsule)
		{
			isHit = CheckCollCS(collB, collA);
		}
		//�|���S��
		else if (shapeB == Shape::Polygon)
		{
			isHit = CheckCollSP(collA, collB);
		}
	}
	//�J�v�Z����
	else if (shapeA == Shape::Capsule)
	{
		//��
		if (shapeB == Shape::Sphere)
		{
			isHit = CheckCollCS(collA, collB);
		}
		//�J�v�Z��
		else if (shapeB == Shape::Capsule)
		{
			isHit = CheckCollCC(collA, collB);
		}
		//�|���S��
		else if (shapeB == Shape::Polygon)
		{
			isHit = CheckCollCP(collA, collB);
		}
	}
	//�|���S����
	else if (shapeA == Shape::Polygon)
	{
		//��
		if (shapeB == Shape::Sphere)
		{
			isHit = CheckCollSP(collB, collA);
		}
		//�J�v�Z��
		else if (shapeB == Shape::Capsule)
		{
			isHit = CheckCollCP(collB, collA);
		}
	}
	return isHit;
}

bool CollisionChecker::CheckCollSS(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//�R���W�����f�[�^�̎擾
	auto collDataA = std::dynamic_pointer_cast<SphereCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<SphereCollider>(collB->m_collisionData);
	//���W�b�h�{�f�B
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;

	//A�̃R���C�_�[�̏��
	Vector3 posA = rbA->GetNextPos();
	float radiusA = collDataA->GetRadius();
	//B�̃R���C�_�[�̏��
	Vector3 posB = rbB->GetNextPos();
	float radiusB = collDataB->GetRadius();

	Vector3 aToB = posB - posA;

	//�Q���̊Ԃ̒��������a�̍��v���傫���Ȃ瓖�����Ă��Ȃ�
	if (aToB.Magnitude() >= radiusA + radiusB)
	{
		return false;
	}

	//�����܂ŗ����瓖�����Ă���
	return true;
}

bool CollisionChecker::CheckCollCS(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//������J�v�Z���ɐ����������ċ��ƃJ�v�Z���̍ŒZ���������߂�

	//�R���C�_�[�f�[�^���擾
	auto collDataA = std::dynamic_pointer_cast<CapsuleCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<SphereCollider>(collB->m_collisionData);
	//���W�b�h�{�f�B
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;

	//�J�v�Z���̂��ꂼ��̍��W
	Vector3 cPosA = rbA->GetNextPos();
	Vector3 cPosB = collDataA->GetNextEndPos(rbA->GetVec());
	//���̍��W
	Vector3 sPos = rbB->GetNextPos();
	//�ŒZ����
	float shortDis = collDataA->m_radius + collDataB->m_radius;

	//�J�v�Z���̍��WA���狅�ւ̃x�N�g��
	Vector3 AtoS = sPos - cPosA;
	//�J�v�Z���̍��WA����B�ւ̃x�N�g��
	Vector3 AtoB = cPosB - cPosA;

	//����
	float dot = AtoS.Dot(AtoB);
	//AB�̒����̓��Ŋ���
	float t = dot / AtoB.SqMagnitude();
	//�N�����v
	t = MathSub::ClampFloat(t, 0.0f, 1.0f);
	//�ŒZ���W���o��
	Vector3 minPos = cPosA + (AtoB * t);
	
	float distance = (sPos - minPos).Magnitude();
	//�ŒZ���傫���Ȃ�
	if (distance >= shortDis)
	{
		return false;
	}
	
	//�Փ˔���Ŏg���̂ň�ԋ߂����W���o���Ă���
	collDataA->SetNearPos(minPos);
	//�����܂ł���Γ������Ă���
	return true;
}

//�Փ˔���̂��߂ɍŋߐړ_�̌v�Z�����Ă�
bool CollisionChecker::CheckCollCC(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	// �ǂ��炩�̃J�v�Z���̈�̍��W����Е��̃J�v�Z���ɐ�����������
	//�@���݂��̃J�v�Z���ɍł��߂����W�����ꂼ��o��

	//�R���C�_�[�f�[�^���擾
	auto collDataA = std::dynamic_pointer_cast<CapsuleCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<CapsuleCollider>(collB->m_collisionData);
	//���W�b�h�{�f�B
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;

	//�J�v�Z���̂��ꂼ��̍��W
	//�J�v�Z��A
	Vector3 capAStartPos = rbA->GetNextPos();
	Vector3 capAEndPos = collDataA->GetNextEndPos(rbA->GetVec());
	float capARadius = collDataA->m_radius;
	//�J�v�Z��B
	Vector3 capBStartPos = rbB->GetNextPos();
	Vector3 capBEndPos = collDataB->GetNextEndPos(rbB->GetVec());
	float capBRadius = collDataB->m_radius;
	
	//���s���ǂ����m�F����
	auto capADir = capAEndPos - capAStartPos;//����1
	auto capBDir = capBStartPos - capBEndPos;//����2
	//���K�����ē��Ȃ��畽�s���`�F�b�N
	auto cross = capADir.Cross(capBDir);
	//���s�Ȃ�
	if (cross.SqMagnitude() <= 0.0f)
	{
		return ParallelCC(collA, collB);
	}

	//�ŒZ����
	float shortDis = capARadius + capBRadius;
	//���̍ŒZ����
	float nowShortDis = 10000.0f;//�n�_���m���ŒZ�ɂ��Ă���

	for (int i = 0; i < 2; ++i)
	{
		//�����̂��ꂼ��̍��W
		Vector3 lineStart;
		Vector3 lineEnd;
		//�ŏ��ɃJ�v�Z��B�ɑ΂��ăJ�v�Z��A�̂��ꂼ��̓_����ŒZ�̍��W���o��
		if (i <= 0)
		{
			//����CD
			lineStart = capBStartPos;
			lineEnd = capBEndPos;
		}
		//���ɂɃJ�v�Z��A�ɑ΂��ăJ�v�Z��B�̂��ꂼ��̓_����ŒZ�̍��W���o��
		else
		{
			//����AB
			lineStart = capAStartPos;
			lineEnd = capAEndPos;
		}

		for (int j = 0; j < 2; ++j)
		{
			//�m�F������W
			Vector3 checkPoint;
			//A��������ɉ��낷
			if (i == 0 && j == 0)checkPoint = capAStartPos;
			//B��������ɉ��낷
			if (i == 0 && j == 1)checkPoint = capAEndPos;
			//C��������ɉ��낷
			if (i == 1 && j == 0)checkPoint = capBStartPos;
			//D��������ɉ��낷
			if (i == 1 && j == 1)checkPoint = capBEndPos;

			//���Ƃ͋��ƃJ�v�Z���̎��Ɠ���
			//A����m�F������W�ւ̃x�N�g��
			Vector3 lineAtoP = checkPoint - lineStart;
			//����AB�̒P�ʃx�N�g��
			Vector3 lineAB = lineEnd - lineStart;

			//���ꂼ��̃x�N�g��������ς������ċ����琂�����������ʒu�����߂�
			float dot = lineAtoP.Dot(lineAB);
			//AB�̒����̓��Ŋ���
			float t = dot / lineAB.SqMagnitude();
			//�N�����v
			t = MathSub::ClampFloat(t, 0.0f, 1.0f);

			//�ŒZ���W���o��
			Vector3 minPos = lineStart + (lineAB * t);
			
			//�ŒZ����
			float distance = (checkPoint - minPos).Magnitude();
			
			//����܂��͑O��̍ŒZ������菬�����Ȃ猻�݂̍ŒZ�����Ƃ���
			if (distance < nowShortDis)
			{
				nowShortDis = distance;

				//�ŒZ���W���L�^
				if (i == 0)
				{
					//�Փ˔���Ŏg���̂ň�ԋ߂����W���o���Ă���
					collDataB->SetNearPos(minPos);
				}
				else
				{
					//�Փ˔���Ŏg���̂ň�ԋ߂����W���o���Ă���
					collDataA->SetNearPos(minPos);
				}
			}
		}
	}

	//���݂̍ŒZ�������ό`�̍��v���傫���Ȃ瓖�����Ă��Ȃ�
	if (nowShortDis >= shortDis)
	{
		return false;
	}


	//�����܂ł���Γ������Ă���
	return true;
}

bool CollisionChecker::CheckCollCCVerDxLib(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//�R���C�_�[�f�[�^���擾
	auto collDataA = std::dynamic_pointer_cast<CapsuleCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<CapsuleCollider>(collB->m_collisionData);
	//���W�b�h�{�f�B
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;

	//�J�v�Z���̂��ꂼ��̍��W
	//�J�v�Z��A
	Vector3 capAStartPos = rbA->GetNextPos();
	Vector3 capAEndPos = collDataA->GetNextEndPos(rbA->GetVec());
	float capARadius = collDataA->m_radius;
	//�J�v�Z��B
	Vector3 capBStartPos = rbB->GetNextPos();
	Vector3 capBEndPos = collDataB->GetNextEndPos(rbB->GetVec());
	float capBRadius = collDataB->m_radius;

	return HitCheck_Capsule_Capsule(capAStartPos.ToDxLibVector(), capAEndPos.ToDxLibVector(), capARadius,
		capBStartPos.ToDxLibVector(), capBEndPos.ToDxLibVector(), capBRadius);
}

bool CollisionChecker::CheckCollSP(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//�R���C�_�[�f�[�^�̎擾
	auto collDataA = std::dynamic_pointer_cast<SphereCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<PolygonCollider>(collB->m_collisionData);
	//���W�b�h�{�f�B
	auto rbA = collA->m_rb;

	//�������Ă�|���S���̐�
	auto hitDim = MV1CollCheck_Sphere(
		collDataB->GetModelHandle(),
		-1,
		rbA->GetNextPos().ToDxLibVector(),
		collDataA->m_radius);
	//����������Ă��Ȃ��Ȃ�false
	if (hitDim.HitNum <= 0 || collA->m_isTrigger)
	{
		// ���o�����v���C���[�̎��͂̃|���S�������J������
		MV1CollResultPolyDimTerminate(hitDim);
		return false;
	}

	//�����蔻��Ɏg���̂ŕۑ�
	collDataB->SetHitDim(hitDim);

	return true;
}

bool CollisionChecker::CheckCollCP(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//�R���C�_�[�f�[�^�̎擾
	auto collDataA = std::dynamic_pointer_cast<CapsuleCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<PolygonCollider>(collB->m_collisionData);
	//���W�b�h�{�f�B
	auto rbA = collA->m_rb;

	//�������Ă�|���S���̐�
	auto hitDim = MV1CollCheck_Capsule(
		collDataB->GetModelHandle(),
		-1,
		rbA->GetNextPos().ToDxLibVector(),
		collDataA->GetNextEndPos(rbA->GetVec()).ToDxLibVector(),
		collDataA->m_radius,
		-1);

	//�������Ă��Ȃ��Ȃ�false
	if (hitDim.HitNum <= 0 || collA->m_isTrigger)
	{
		// ���o�����v���C���[�̎��͂̃|���S�������J������
		MV1CollResultPolyDimTerminate(hitDim);
		return false;
	}
	//�����蔻��Ɏg���̂ŕۑ�
	collDataB->SetHitDim(hitDim);

	return true;
}

bool CollisionChecker::ParallelCC(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//�R���C�_�[�f�[�^���擾
	auto collDataA = std::dynamic_pointer_cast<CapsuleCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<CapsuleCollider>(collB->m_collisionData);
	//���W�b�h�{�f�B
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;

	//�J�v�Z��A
	Vector3 cPosA = rbA->GetNextPos();
	Vector3 cPosB = collDataA->GetNextEndPos(rbA->GetVec());
	//�J�v�Z��B
	Vector3 cPosC = rbB->GetNextPos();
	Vector3 cPosD = collDataB->GetNextEndPos(rbB->GetVec());
	//�ŒZ����
	float shortDis = collDataA->m_radius + collDataB->m_radius;
	
	//�e�������`�F�b�N
	Vector3 ac = cPosC - cPosA;
	Vector3 ad = cPosD - cPosA;
	Vector3 bc = cPosC - cPosB;
	Vector3 bd = cPosD - cPosB;
	//�ŒZ�������o��
	float dis = ac.Magnitude();
	//��x����Ă���
	collDataA->SetNearPos(cPosA);
	collDataB->SetNearPos(cPosC);
	//�Z���Ȃ�
	if (dis > ad.Magnitude())
	{
		dis = ad.Magnitude();
		collDataA->SetNearPos(cPosA);
		collDataB->SetNearPos(cPosD);
	}
	if (dis > bc.Magnitude())
	{
		dis = bc.Magnitude();
		collDataA->SetNearPos(cPosB);
		collDataB->SetNearPos(cPosC);
	}
	if (dis > bd.Magnitude())
	{
		dis = bd.Magnitude();
		collDataA->SetNearPos(cPosB);
		collDataB->SetNearPos(cPosD);
	}

	//�ŒZ�������傫���Ȃ瓖�����Ă��Ȃ�
	if (dis >= shortDis)
	{
		return false;
	}

	//�����܂ŗ����瓖�����Ă�
	return true;
}