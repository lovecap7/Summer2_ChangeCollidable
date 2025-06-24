#pragma once
#include <memory>
#include <vector>
class Actor;
class Player;
class Input;
class Camera;
class AttackManager;
class CollisionManager;
class UIManager;
class EnemyManager;
class PlayerManager;
class ItemGenerator;
class StageSetup;
class ActorManager : public std::enable_shared_from_this<ActorManager>
{
public:
	ActorManager(std::shared_ptr<Player> player);
	virtual~ActorManager();
	//�A�N�^�[�̓o�^����
	void Entry(std::shared_ptr<Actor> actor);
	//�o�^����
	void Exit(std::shared_ptr<Actor> actor);
	//����������
	void Init(std::shared_ptr<StageSetup> setup);
	//�I������
	void End();
	//�X�V����
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<UIManager> uiManager);
	//�`��
	void Draw()const;
	//�V�����A�N�^�[��ǉ�����
	void SetNewActor(std::shared_ptr<Actor> actor);
	//�R���W�����}�l�[�W���[
	std::shared_ptr<CollisionManager> GetCollisionManager() const{ return m_collManager; };
	//�G�l�~�[�}�l�[�W���[
	std::shared_ptr<EnemyManager> GetEnemyManager() const{ return m_enemyManager; };
	//�v���C���[�}�l�[�W���[
	std::shared_ptr<PlayerManager> GetPlayerManager() const{ return m_playerManager; };
	//�U���}�l�[�W���[
	std::shared_ptr<AttackManager> GetAttackManager() const { return m_attackManager; };
	//�A�C�e���W�F�l���[�^�[
	std::shared_ptr<ItemGenerator> GetItemGenerator() const { return m_itemGenerator; };
private:
	//�A�N�^�[
	std::vector<std::shared_ptr<Actor>> m_actors;
	//�ǉ��\��̃A�N�^�[
	std::vector<std::shared_ptr<Actor>> m_addActors;
	//�v���C���[
	std::shared_ptr<Player> m_player;
	//�����蔻��ƏՓˏ���
	std::shared_ptr<CollisionManager> m_collManager;
	//�G�̕⏕�I�ȏ���������
	std::shared_ptr<EnemyManager> m_enemyManager;
	//�v���C���[�̕⏕�I�ȏ���������
	std::shared_ptr<PlayerManager> m_playerManager;
	//�U���̏���
	std::shared_ptr<AttackManager> m_attackManager;
	//�A�C�e���W�F�l���[�^�[
	std::shared_ptr<ItemGenerator> m_itemGenerator;
private:
	int m_id;//����U��ԍ�
	void SetUpId(std::shared_ptr<Actor> actor);
	//���Ńt���O�������Ă���A�N�^�[�̍폜
	void CheckDeleteActor(std::shared_ptr<ItemGenerator> itemGenerator);
	//�V�����A�N�^�[�̎���
	void AddNewActors();
};

