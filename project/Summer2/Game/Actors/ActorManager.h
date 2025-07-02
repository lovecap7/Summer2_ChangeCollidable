#pragma once
#include <list>
#include <memory>
#include "Attack/AttackBase.h"
#include "Item/ItemBase.h"
class Player;
class Camera;
class Actor;
class StageSetup;
class BossArea;
class EnemyBase;
class ActorManager : public std::enable_shared_from_this<ActorManager>
{
public:
	ActorManager();
	~ActorManager();
	//������
	void Init(std::unique_ptr<StageSetup>& stageSetup);
	//�X�V
	void Update(const std::weak_ptr<Camera> camera);
	//�`��
	void Draw() const;
	//�I������
	void End();
	//�v���C���[���擾
	std::weak_ptr<Player> GetPlayer() const { return m_player; }
	//�{�X���擾
	std::weak_ptr<EnemyBase> GetBoss() const { return m_boss; }
	//�v���C���[�ɋ߂��G���擾
	std::weak_ptr<Actor> GetNearestEnemy() const;
	//�{�X�����ɓ�������
	bool IsEntryBossArea()const { return m_bossArea.expired(); };
	//�ǉ��\��̃A�N�^�[��ǉ�
	void AddNextActor(std::shared_ptr<Actor> actor);
	//�U�����쐬���ĎQ�Ƃ�Ԃ�
	std::weak_ptr<AttackBase> CreateAttack(AttackType at, std::weak_ptr<Actor> owner);
	//�A�C�e���̒ǉ�
	std::weak_ptr<ItemBase> CreateItem(ItemType it, Vector3 pos);
	//�G���G�����ׂč폜
	void AllDeleteNormalEnemy();
private:
	//�A�N�^�[��ǉ�
	void AddActor(std::shared_ptr<Actor> actor);
	//�ǉ��\��̃A�N�^�[������
	void CheckNextAddActors();
	//�폜�\��̃A�N�^�[���폜
	void CheckDeleteActors();
private:
	//�A�N�^�[
	std::list<std::shared_ptr<Actor>> m_actors;
	//�ǉ��\��̃A�N�^�[
	std::list<std::shared_ptr<Actor>> m_nextAddActors;
	//�v���C���[
	std::weak_ptr<Player> m_player;
	//�{�X
	std::weak_ptr<EnemyBase> m_boss;
	//�{�X����
	std::weak_ptr<BossArea> m_bossArea;
	//ID
	int m_actorId;//����U��ԍ�
private:
	//�n���h��
	int m_heartHandle;			//��
	int m_bombHandle;			//���e
	int m_ultGageUpHandle;		//�K�E�Q�[�W�A�b�v
	int m_attackUpHandle;		//�U���A�b�v
	int m_defenseUpHandle;		//�ϋv�A�b�v
};

