#pragma once
#include "../Actor.h"
#include <memory>
#include "../../../General/Model.h"
class Trigger;
class AttackBase;
class AttackManager;
class SearchTrigger;
class EnemyBase abstract :
    public Actor
{
public:
    EnemyBase();
    virtual ~EnemyBase() {};
    //�v���C���[�ւ̃x�N�g����Ԃ��֐�
    Vector3 GetPlayerVec() const;
	//�v���C���[�̈ʒu��XZ���ʏ�̃x�N�g���ɂ��ĕԂ��֐�
    Vector3 GetPlayerNomVecXZ() const;
    //���G�͈�
    std::shared_ptr<SearchTrigger> GetSearchTrigger() { return m_searchTrigger; };
    //���f���N���X
    std::shared_ptr<Model> GetModel() const { return m_model; };
protected:
    //���f��
    std::shared_ptr<Model> m_model;
    //���G�g���K�[
    std::shared_ptr<SearchTrigger> m_searchTrigger;
    //���ꔻ��̍X�V
    virtual void UpdateHurtPoint()abstract;
    //�U��������o��
    void AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager> attackManager);
};

