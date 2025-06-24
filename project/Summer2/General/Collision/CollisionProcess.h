#pragma once
#include <memory>
#include <DxLib.h>
#include "../Math/MyMath.h"
namespace
{
	//�|���S���̓����蔻��̔z��̍ő吔
	constexpr int kMaxHitPolygon = 2048;
}
class Collidable;
class CollisionProcess
{
public:
	CollisionProcess();
	~CollisionProcess();
	/// <summary>
	/// ���Ƌ��̏Փˏ���
	/// </summary>
	/// <param name="otherA">��</param>
	/// <param name="otherB">��</param>
	void ProcessSS(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
	/// <summary>
	/// ���ƃ|���S���̏Փˏ���
	/// </summary>
	/// <param name="otherA">��</param>
	/// <param name="otherB">�|���S��</param>
	void ProcessSP(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
	/// <summary>
	/// �J�v�Z���ƃJ�v�Z���̏Փˏ���
	/// </summary>
	/// <param name="otherA">�J�v�Z��</param>
	/// <param name="otherB">�J�v�Z��</param>
	void ProcessCC(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
	/// <summary>
	/// �J�v�Z���Ƌ�
	/// </summary>
	/// <param name="otherA">�J�v�Z��</param>
	/// <param name="otherB">��</param>
	void ProcessCS(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
	/// <summary>
	/// �J�v�Z���ƃ|���S���̏Փˏ���
	/// </summary>
	/// <param name="otherA">�J�v�Z��</param>
	/// <param name="otherB">�|���S��</param>
	void ProcessCP(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
private:
	int	m_wallNum;			// �ǃ|���S���Ɣ��f���ꂽ�|���S���̐�
	int	m_floorAndRoofNum;			// ���|���S���Ɣ��f���ꂽ�|���S���̐�
	MV1_COLL_RESULT_POLY* m_wall[kMaxHitPolygon];
	MV1_COLL_RESULT_POLY* m_floorAndRoof[kMaxHitPolygon];
	/// <summary>
	/// ���|���S���ƕǃ|���S���ɕ�����
	/// </summary>
	void AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const Vector3& oldPos);
	/// <summary>
	/// ���ƃ|���S���̉����߂��x�N�g����Ԃ�
	/// </summary>
	Vector3 OverlapVecSphereAndPoly(int hitNum, const Vector3& nextPos, MV1_COLL_RESULT_POLY* dim, float shortDis);
	/// <summary>
	/// �ǂƓ����������̏��� �J�v�Z��
	/// </summary>
	Vector3 HitWallCP(const Vector3& headPos, const Vector3& legPos, int hitNum, MV1_COLL_RESULT_POLY* dim, float shortDis);
	/// <summary>
	/// ���̍����ɍ��킹�� �J�v�Z��
	/// </summary>
	bool HitFloorCP(const std::shared_ptr<Collidable>& other, const Vector3& legPos, int hitNum, MV1_COLL_RESULT_POLY* dim, float shortDis);
	/// <summary>
	/// �V��ɓ����������̏��� �J�v�Z��
	/// </summary>
	void HitRoofCP(const std::shared_ptr<Collidable>& other, const Vector3& headPos, int hitNum, MV1_COLL_RESULT_POLY* dim, float shortDis);
};

