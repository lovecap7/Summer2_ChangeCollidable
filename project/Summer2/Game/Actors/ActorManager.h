#pragma once
#include <list>
#include <memory>

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
	//�ǉ��\��̃A�N�^�[��ǉ�
	void AddNextActor(std::shared_ptr<Actor> actor);
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
	std::shared_ptr<StageSetup> m_stageSetup;
	//ID
	int m_actorId;//����U��ԍ�
};

