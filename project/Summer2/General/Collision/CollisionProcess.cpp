#include "CollisionProcess.h"
#include "Collidable.h"
#include "../Rigidbody.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PolygonCollider.h"
#include <vector>

namespace
{
	//�����߂��̒l�ɑ����Ė�������̂�h��
	constexpr float kOverlapGap = 1.0f;
	constexpr float kCheckUnder = -800.0f;
	constexpr float kCheckTop = 800.0f;
}

CollisionProcess::CollisionProcess() :
	m_wallNum(0),
	m_floorAndRoofNum(0),
	m_wall{ nullptr },
	m_floorAndRoof{ nullptr }
{
}

CollisionProcess::~CollisionProcess()
{
}

void CollisionProcess::FixNextPos(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//�Փ˂��Ă���I�u�W�F�N�g�̌`����擾
	auto collAShape = collA->m_collisionData->m_shape;
	auto collBShape = collB->m_collisionData->m_shape;
	//����
	if (collAShape == Shape::Sphere)
	{
		//��
		if (collBShape == Shape::Sphere)
		{
			//�x�N�g����␳����
			ProcessSS(collA, collB);
		}
		//�J�v�Z��
		else if (collBShape == Shape::Capsule)
		{
			//�x�N�g����␳����
			ProcessCS(collB, collA);
		}
		//�|���S��
		else if (collBShape == Shape::Polygon)
		{
			//�x�N�g����␳����
			ProcessSP(collA, collB);
		}
	}
	//�J�v�Z����
	else if (collAShape == Shape::Capsule)
	{
		//��
		if (collBShape == Shape::Sphere)
		{
			//�x�N�g����␳����
			ProcessCS(collA, collB);
		}
		//�J�v�Z��
		else if (collBShape == Shape::Capsule)
		{
			//�x�N�g����␳����
			ProcessCC(collA, collB);
		}
		//�|���S��
		else if (collBShape == Shape::Polygon)
		{
			//�x�N�g����␳����
			ProcessCP(collA, collB);
		}
	}
	//�|���S����
	else if (collAShape == Shape::Polygon)
	{
		//��
		if (collBShape == Shape::Sphere)
		{
			//�x�N�g����␳����
			ProcessSP(collB, collA);
		}
		//�J�v�Z��
		else if (collBShape == Shape::Capsule)
		{
			//�x�N�g����␳����
			ProcessCP(collB, collA);
		}
	}
}

void CollisionProcess::ProcessSS(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//�D��x
	auto priorityA = collA->m_priority;
	auto priorityB = collB->m_priority;
	//���݂������Ȃ��I�u�W�F�N�g�Ȃ�Փ˂��Ȃ�
	if (priorityA == Priority::Static &&
		priorityB == Priority::Static)return;

	//���W�b�h�{�f�B
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;

	//A����B�ւ̃x�N�g��
	Vector3 aToB = rbB->GetNextPos() - rbA->GetNextPos();
	//�ŒZ����
	float shortDis = std::dynamic_pointer_cast<SphereCollider> (collA->m_collisionData)->GetRadius() + std::dynamic_pointer_cast<SphereCollider> (collB->m_collisionData)->GetRadius();
	//�ǂ̂��炢�d�˂��Ă��邩
	float overlap = shortDis - aToB.Magnitude();
	overlap = MathSub::ClampFloat(overlap, 0, shortDis);
	overlap += kOverlapGap;

	//�D��x���瓮�����ق������߂�
	if (priorityA > priorityB)
	{
		rbB->AddVec(aToB.Normalize() * overlap);
	}
	else if (priorityA < priorityB)
	{
		rbA->AddVec(aToB.Normalize() * -overlap);
	}
	else
	{
		rbA->AddVec(aToB.Normalize() * -overlap / 2.0f);
		rbB->AddVec(aToB.Normalize() * overlap / 2.0f);
	}
}

void CollisionProcess::ProcessSP(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//�R���W�����f�[�^
	auto collDataA = std::dynamic_pointer_cast<SphereCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<PolygonCollider>(collB->m_collisionData);
	//���W�b�h�{�f�B
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;
	
	//���������|���S���̏��
	auto hitDim = collDataB->GetHitDim();

	//���݂������Ȃ��I�u�W�F�N�g�Ȃ�Փ˂��Ȃ�(�|���S���̓X�^�e�B�b�N�Ȃ̂ŕЕ����X�^�e�B�b�N�Ȃ�)
	if (collA->m_priority == Priority::Static)
	{
		// ���o�����v���C���[�̎��͂̃|���S�������J������
		DxLib::MV1CollResultPolyDimTerminate(hitDim);
		return;
	}

	//���̍��W
	Position3 nextPos = rbA->GetNextPos();//�ړ���

	//���|���S���ƕǃ|���S���ɕ�����
	AnalyzeWallAndFloor(hitDim, nextPos);
	//���Ɠ��������Ȃ�
	if (m_floorAndRoofNum > 0)
	{
		//�␳����x�N�g����Ԃ�
		Vector3 overlapVec = OverlapVecSphereAndPoly(m_floorAndRoofNum, nextPos, *m_floorAndRoof, collDataA->m_radius);
	
		//�|���S���͌Œ�(static)�Ȃ̂ŋ��̂ݓ�����
		rbA->AddVec(overlapVec);
		//�C��������������Ȃ珰
		if (overlapVec.y > 0)
		{
			//���ɓ������Ă���̂�
			collA->SetIsFloor(true);
		}
	}

	//�ǂƓ������Ă���Ȃ�
	if(m_wallNum > 0)
	{
		//�ǂɓ������Ă���̂�
		collA->SetIsWall(true);

		//�␳����x�N�g����Ԃ�
		Vector3 overlapVec = OverlapVecSphereAndPoly(m_wallNum, nextPos, *m_wall, collDataA->m_radius);
		
		//�|���S���͌Œ�(static)�Ȃ̂ŋ��̂ݓ�����
		rbA->AddVec(overlapVec);
	}

	// ���o�����v���C���[�̎��͂̃|���S�������J������
	DxLib::MV1CollResultPolyDimTerminate(hitDim);
}

void CollisionProcess::ProcessCC(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//�D��x
	auto priorityA = collA->m_priority;
	auto priorityB = collB->m_priority;
	//���݂������Ȃ��I�u�W�F�N�g�Ȃ�Փ˂��Ȃ�
	if (priorityA == Priority::Static &&
		priorityB == Priority::Static)return;

	//�J�v�Z���̉����߂��͂��ꂼ��̓��������|�C���g����v�Z���܂�

	//�R���C�_�[�f�[�^
	auto collDataA = std::dynamic_pointer_cast<CapsuleCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<CapsuleCollider>(collB->m_collisionData);
	//���W�b�h�{�f�B
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;

	//A����B�ւ̃x�N�g��
	Vector3 aToB = collDataB->GetNearPos() - collDataA->GetNearPos();
	//�ŒZ����
	float shortDis = collDataA->m_radius + collDataB->m_radius;
	//�ǂ̂��炢�d�˂��Ă��邩
	float overlap = shortDis - aToB.Magnitude();
	overlap = MathSub::ClampFloat(overlap, 0, shortDis);
	overlap += kOverlapGap;

	//�������ɂ��������������̂�
	aToB.y = 0.0f;

	//���������̂Ƃ�������Ȃ����̂Ƃŏ����𕪂���
	if (priorityA > priorityB)
	{
		rbB->AddVec(aToB.Normalize() * overlap);
	}
	else if (priorityA < priorityB)
	{
		rbA->AddVec(aToB.Normalize() * -overlap);
	}
	else
	{
		rbA->AddVec(aToB.Normalize() * -overlap / 2.0f);
		rbB->AddVec(aToB.Normalize() * overlap / 2.0f);
	}
}

void CollisionProcess::ProcessCS(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//�D��x
	auto priorityA = collA->m_priority;
	auto priorityB = collB->m_priority;
	//���݂������Ȃ��I�u�W�F�N�g�Ȃ�Փ˂��Ȃ�
	if (priorityA == Priority::Static &&
		priorityB == Priority::Static)return;
	//�R���C�_�[�f�[�^
	auto collDataA = std::dynamic_pointer_cast<CapsuleCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<SphereCollider>(collB->m_collisionData);
	//���W�b�h�{�f�B
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;

	//A����B�ւ̃x�N�g��
	Vector3 aToB = rbB->GetNextPos() - collDataA->GetNearPos();

	//�ŒZ����
	float shortDis = collDataB->m_radius + collDataA->GetRadius();
	//�ǂ̂��炢�d�˂��Ă��邩
	float overlap = shortDis - aToB.Magnitude();
	overlap = MathSub::ClampFloat(overlap, 0, shortDis);
	overlap += kOverlapGap;

	//���������̂Ƃ�������Ȃ����̂Ƃŏ����𕪂���
	if (priorityA > priorityB)
	{
		rbB->AddVec(aToB.Normalize() * overlap);
	}
	else if (priorityA < priorityB)
	{
		rbA->AddVec(aToB.Normalize() * -overlap);
	}
	else
	{
		rbA->AddVec(aToB.Normalize() * -overlap / 2.0f);
		rbB->AddVec(aToB.Normalize() * overlap / 2.0f);
	}
}

void CollisionProcess::ProcessCP(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//�R���C�_�[�f�[�^
	auto collDataA = std::dynamic_pointer_cast<CapsuleCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<PolygonCollider>(collB->m_collisionData);
	//���W�b�h�{�f�B
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;

	//���������|���S���̏��
	auto hitDim = collDataB->GetHitDim();
	//���݂������Ȃ��I�u�W�F�N�g�Ȃ�Փ˂��Ȃ�(�|���S���̓X�^�e�B�b�N�Ȃ̂ŕЕ����X�^�e�B�b�N�Ȃ�)
	if (collA->m_priority == Priority::Static)
	{
		// ���o�����v���C���[�̎��͂̃|���S�������J������
		DxLib::MV1CollResultPolyDimTerminate(hitDim);
		return;
	}

	//�J�v�Z���̓����W�Ƒ����W
	Position3 headPos = collDataA->GetNextEndPos(rbA->GetVec());//�ړ���
	Position3 legPos = rbA->GetNextPos();//�ړ���
	//����葫�̂ق����Ⴂ�ʒu�ɂ���Ȃ����ւ���
	if (headPos.y < legPos.y)
	{
		Position3 temp = legPos;
		legPos = headPos;
		headPos = temp;
	}

	//���|���S���ƕǃ|���S���ɕ�����
	AnalyzeWallAndFloor(hitDim, legPos);
	//���Ɠ��������Ȃ�
	if (m_floorAndRoofNum > 0)
	{
		//�W�����v���Ă���Ȃ�
		if (collA->m_collState == CollisionState::Jump)
		{
			//�V��ɓ�����������
			HitRoofCP(collA, headPos, m_floorAndRoofNum, *m_floorAndRoof, collDataA->m_radius);
		}
		else
		{
			//���̍����ɍ��킹��
			if (HitFloorCP(collA, legPos, m_floorAndRoofNum, *m_floorAndRoof, collDataA->m_radius))
			{
				//���ɓ������Ă���̂�
				collA->SetIsFloor(true);
			}
		}
	}

	//�ǂƓ������Ă���Ȃ�
	if (m_wallNum > 0)
	{
		//�ǂɓ������Ă���̂�
		collA->SetIsWall(true);

		//�␳����x�N�g����Ԃ�
		Vector3 overlapVec = HitWallCP(headPos, legPos, m_wallNum, *m_wall, collDataA->GetRadius());
		
		//�|���S���͌Œ�(static)�Ȃ̂ŋ��̂ݓ�����
		rbA->AddVec(overlapVec);
	}

	// ���o�����v���C���[�̎��͂̃|���S�������J������
	DxLib::MV1CollResultPolyDimTerminate(hitDim);

}

void CollisionProcess::AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const Vector3& nextPos)
{
	//�ǃ|���S���Ə��|���S���̐�������������
	m_wallNum = 0;
	m_floorAndRoofNum = 0;

	//���o���ꂽ�|���S���̐������J��Ԃ�
	for (int i = 0; i < hitDim.HitNum;++i)
	{
		//XZ���ʂɐ������ǂ����̓|���S���̖@����Y������0�ɋ߂����ǂ����Ŕ��f����
		if (hitDim.Dim[i].Normal.y < 0.1f && hitDim.Dim[i].Normal.y > -0.1f)
		{
			//�ǃ|���S���Ɣ��f���ꂽ�ꍇ�ł��A�v���C���[��Y���W�{1.0f��荂���|���S���̂ݓ����蔻����s��
			//�i���œ˂�������̂�h������
			if (hitDim.Dim[i].Position[0].y > nextPos.y + 1.0f ||
				hitDim.Dim[i].Position[1].y > nextPos.y + 1.0f ||
				hitDim.Dim[i].Position[2].y > nextPos.y + 1.0f)
			{
				//�|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɕۑ�����
				if (m_wallNum < kMaxHitPolygon)
				{
					//�|���S���̍\���̂̃A�h���X��ǃ|���S���z��ɕۑ�
					m_wall[m_wallNum] = &hitDim.Dim[i];
					++m_wallNum;
				}
			}
		}
		//���|���S��
		else
		{
			//�|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S���z��ɕۑ�
			if (m_floorAndRoofNum < kMaxHitPolygon)
			{
				//�|���S���̍\���̂̃A�h���X�����|���S���z��ɕۑ�
				m_floorAndRoof[m_floorAndRoofNum] = &hitDim.Dim[i];
				++m_floorAndRoofNum;
			}
		}
	}
}

Vector3 CollisionProcess::OverlapVecSphereAndPoly(int hitNum ,const Vector3& nextPos,MV1_COLL_RESULT_POLY* dim ,float shortDis)
{
	//�����������ċ߂��_��T���čŒZ���������߂�
	float hitShortDis = 0;//�ŒZ����
	//�@��
	Vector3 nom = {};
	for (int i = 0; i < hitNum; ++i)
	{
		//���ςƖ@���x�N�g�����瓖�����Ă���W�����߂�
		VECTOR bToA = VSub(nextPos.ToDxLibVector(), dim[i].Position[0]);
		float dot = VDot(dim[i].Normal, bToA);

		//�|���S���Ɠ��������I�u�W�F�N�g���@�������ɂ���Ȃ�����𔽓]
		if ((bToA.y > 0 && dim[i].Normal.y > 0) || (bToA.y < 0 && dim[i].Normal.y < 0))
		{
			dot *= -1;
		}
		//�����������W
		VECTOR hitPos = VAdd(VScale(dim[i].Normal, dot), nextPos.ToDxLibVector());
		//����
		float dis = VSize(VSub(hitPos, nextPos.ToDxLibVector()));
		//����܂��͑O���苗�����Z���Ȃ�
		if (i <= 0 || hitShortDis > dis)
		{
			//����̍ŒZ
			hitShortDis = dis;
			//�@��
			nom = Vector3{ dim[i].Normal.x,dim[i].Normal.y ,dim[i].Normal.z };
		}
	}
	//�����߂�
	//�ǂꂭ�炢�����߂���
	float overlap = shortDis - hitShortDis;
	overlap = MathSub::ClampFloat(overlap, 0, shortDis);
	overlap += kOverlapGap;

	return nom.Normalize() * overlap;
}

Vector3 CollisionProcess::HitWallCP(const Vector3& headPos, const Vector3& legPos, int hitNum, MV1_COLL_RESULT_POLY* dim, float shortDis)
{
	//�����������ċ߂��_��T���čŒZ���������߂�
	float hitShortDis = shortDis;//�ŒZ����
	//�@��
	Vector3 nom = {};
	for (int i = 0; i < hitNum; ++i)
	{
		//�ŒZ������2���Ԃ�
		float dis = Segment_Triangle_MinLength_Square(headPos.ToDxLibVector(), legPos.ToDxLibVector(), dim[i].Position[0], dim[i].Position[1], dim[i].Position[2]);
		//��������Ԃ�
		dis = sqrtf(dis);
		//����܂��͑O���苗�����Z���Ȃ�
		if (hitShortDis > dis)
		{
			//����̍ŒZ
			hitShortDis = dis;
			//�@��
			nom = Vector3{ dim[i].Normal.x,0.0f ,dim[i].Normal.z };
		}
	}
	//�����߂�
	//�ǂꂭ�炢�����߂���
	float overlap = shortDis - hitShortDis;
	overlap = MathSub::ClampFloat(overlap, 0, shortDis);
	overlap += kOverlapGap;
	//���K��
	if (nom.Magnitude() != 0.0f)
	{
		nom = nom.Normalize();
	}
	return nom * overlap;
}


bool CollisionProcess::HitFloorCP(const std::shared_ptr<Collidable> coll, const Vector3& legPos, int hitNum, MV1_COLL_RESULT_POLY* dim, float shortDis)
{
	//���W�b�h�{�f�B
	auto rb = coll->m_rb;

	//�����������ċ߂��_��T���čŒZ���������߂�
	float hitShortDis = shortDis;//�ŒZ����
	//�����������ő����Ɉ�ԋ߂�Y���W�ɍ��킹��
	float lowHitPosY = rb->GetPos().y;
	//���Ɠ���������
	bool hitFloor = false;
	for (int i = 0; i < hitNum; ++i)
	{
		//�������̖@���x�N�g���Ȃ��΂�
		if (dim[i].Normal.y < 0.0f)continue;
		// ���̉��Ƀ|���S�������邩���`�F�b�N
		 HITRESULT_LINE lineResult = HitCheck_Line_Triangle(legPos.ToDxLibVector(), VAdd(legPos.ToDxLibVector(), VGet(0.0f, kCheckUnder, 0.0f)), dim[i].Position[0], dim[i].Position[1], dim[i].Position[2]);

		if (lineResult.HitFlag)
		{
			hitFloor = true;
			 //����
			 float dis = VSize(VSub(lineResult.Position, legPos.ToDxLibVector()));
			 //����܂��͑O���苗�����Z���Ȃ�
			 if (hitShortDis > dis)
			 {
				 //����̍ŒZ
				 hitShortDis = dis;
				 lowHitPosY = lineResult.Position.y;
			 }
		}
	}
	//������������Ȃ�
	if (hitFloor)
	{
		//���̍����ɍ��킹��
		lowHitPosY += shortDis + kOverlapGap;
		rb->SetPosY(lowHitPosY);
		rb->SetVecY(0.0f);
	}
	return hitFloor;
}

void CollisionProcess::HitRoofCP(const std::shared_ptr<Collidable> coll, const Vector3& headPos, int hitNum, MV1_COLL_RESULT_POLY* dim, float shortDis)
{
	//���W�b�h�{�f�B
	auto rb = coll->m_rb;
	//�����������ċ߂��_��T���čŒZ���������߂�
	float hitShortDis = shortDis;//�ŒZ����
	//�����������ő����Ɉ�ԋ߂�Y���W�ɍ��킹��
	float lowHitPosY = rb->GetPos().y;
	//�V��Ɠ���������
	bool hitRoof = false;
	for (int i = 0; i < hitNum; ++i)
	{
		//������̖@���x�N�g���Ȃ��΂�
		if (dim[i].Normal.y > 0.0f)continue;
		// ���̏�Ƀ|���S�������邩���`�F�b�N
		HITRESULT_LINE lineResult = HitCheck_Line_Triangle(headPos.ToDxLibVector(), VAdd(headPos.ToDxLibVector(), VGet(0.0f, kCheckTop, 0.0f)), dim[i].Position[0], dim[i].Position[1], dim[i].Position[2]);

		if (lineResult.HitFlag)
		{
			hitRoof = true;
			//����
			float dis = VSize(VSub(lineResult.Position, headPos.ToDxLibVector()));
			//����܂��͑O���苗�����Z���Ȃ�
			if (hitShortDis > dis)
			{
				//����̍ŒZ
				hitShortDis = dis;
			}
		}
	}
	//������������Ȃ�
	if (hitRoof)
	{
		//�����߂�
		//�ǂꂭ�炢�����߂���
		float overlap = shortDis - hitShortDis;
		overlap = MathSub::ClampFloat(overlap, 0, shortDis);
		overlap += kOverlapGap;
		//�@��
		Vector3 nom = { 0.0f,-1.0f,0.0f };
		//�͂�^����
		rb->AddVec(nom * overlap);
	}
}


