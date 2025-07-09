#pragma once
#include "../CharacterBase.h"
#include <memory>
#include "../../../../General/Model.h"
class Player;
class HitPoints;
//�G�̊K��
enum class EnemyGrade
{
	Normal,   //�G���G
	Elite,    //���{�X
	Boss      //�{�X
};
class EnemyBase abstract :
    public CharacterBase
{
public:
    EnemyBase(Shape shape, EnemyGrade grade);
    virtual ~EnemyBase() {};
	//�U���̃N�[���^�C�����X�V
	virtual void UpdateAttackCoolTime();
	//�U���̃N�[���^�C�����擾
	int GetAttackCoolTime() const { return m_attackCoolTime; }
	//�U���̃N�[���^�C�����Z�b�g
	void SetAttackCoolTime(int coolTime) { m_attackCoolTime = coolTime; }
	//�G�̗͕̑\���̍��W
	Vector3 GetHPUIPos() { return m_hpUIPos; };
	//�G�̊K��
	EnemyGrade GetEnemyGrade()const { return m_enemyGrade; };
	//�v���C���[�𔭌��ł�����
	bool IsSearch()const { return m_targetData.isHitTarget; };
protected:
	//�U���ł���܂ł̃N�[���^�C��
	int m_attackCoolTime;
	//���̍��W
	Vector3 m_hpUIPos;
	//�����̊K��
	EnemyGrade m_enemyGrade;
};

