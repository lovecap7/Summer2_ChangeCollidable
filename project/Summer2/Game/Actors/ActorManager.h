#pragma once
#include <list>
#include <memory>
#include "Attack/AttackBase.h"
#include "Item/ItemBase.h"
class Player;
class Camera;
class Actor;
class StageSetup;
class ActorManager : public std::enable_shared_from_this<ActorManager>
{
public:
	ActorManager();
	~ActorManager();
	//������
	void Init();
	//�X�V
	void Update(const std::weak_ptr<Camera> camera);
	//�`��
	void Draw() const;
	//�I������
	void End();
	//�v���C���[���擾
	std::weak_ptr<Player> GetPlayer() const { return m_player; }
	//�v���C���[�ɋ߂��G���擾
	std::weak_ptr<Actor> GetNearestEnemy() const;
	//�ǉ��\��̃A�N�^�[��ǉ�
	void AddNextActor(std::shared_ptr<Actor> actor);
	//�U�����쐬���ĎQ�Ƃ�Ԃ�
	std::weak_ptr<AttackBase> CreateAttack(AttackType at, std::weak_ptr<Actor> owner);
	//�A�C�e���̒ǉ�
	std::weak_ptr<ItemBase> CreateItem(ItemType it, Vector3 pos);
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
	std::shared_ptr<Player> m_player;
	//�X�e�[�W�Z�b�g�A�b�v
	std::unique_ptr<StageSetup> m_stageSetup;
	//ID
	int m_actorId;//����U��ԍ�
private:
	//�n���h��
	int m_heartHandle;			//��
	int m_bombHandle;			//���e
	int m_ultGageUpHandle;		//�K�E�Q�[�W�A�b�v
	int m_defenseHandle;		//�ϋv�A�b�v
};

