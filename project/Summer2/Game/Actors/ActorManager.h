#pragma once
#include <list>
#include <memory>
#include "Attack/AttackBase.h"
#include "Item/ItemBase.h"
#include "../../General/CSVDataLoader.h"
#include "../../Scene/StageScene.h"

//�L�����N�^�[�̎��
enum class CharacterType : int
{
	Player			= 0,
	PurpleDinosaur	= 1,
	SmallDragon		= 2,
	Bomber			= 3,
	BossDragon		= 4,
};
//�A�C�e���̎��
enum class ItemType : int
{
	Heart			= 0,
	Bomb			= 1,
	UltGageUp		= 2,
	AttackUp		= 3,
	DefenseUp		= 4,
};
//�U���̎��
enum class AttackType : int
{
	Slash			= 0,
	Strike			= 1,
	AreaOfEffect	= 2,
	Bullet			= 3,
	Blast			= 4,
	Breath			= 5,
};

class Player;
class Camera;
class Actor;
class BossArea;
class EnemyBase;
class UIManager;
class Score;
class CharacterBase;
class ActorManager : public std::enable_shared_from_this<ActorManager>
{
public:
	ActorManager(Stage::StageIndex index,std::weak_ptr<UIManager> uiManager);
	~ActorManager();
	//������
	void Init();
	//�X�V
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<Score> score);
	//�`��
	void Draw() const;
	//�I������
	void End();

	//�ǉ��\��̃A�N�^�[��ǉ�
	void AddNextActor(std::shared_ptr<Actor> actor);
	//�L�����N�^�[���쐬
	std::weak_ptr<CharacterBase> CreateCharacter(CharacterType ch, Vector3 pos);
	//�U�����쐬���ĎQ�Ƃ�Ԃ�
	std::weak_ptr<AttackBase> CreateAttack(AttackType at, std::weak_ptr<Actor> owner);
	//�A�C�e���̒ǉ�
	std::weak_ptr<ItemBase> CreateItem(ItemType it, Vector3 pos);
	//�G���G�����ׂč폜
	void AllDeleteNormalEnemy();

	//�v���C���[���擾
	std::weak_ptr<Player> GetPlayer() const { return m_player; }
	//�{�X���擾
	std::weak_ptr<EnemyBase> GetBoss() const { return m_boss; }
	//�v���C���[�ɋ߂��G���擾
	std::weak_ptr<Actor> GetNearestEnemy() const;
	//�{�X�����ɓ�������
	bool IsEntryBossArea()const { return m_bossArea.expired(); };
	//�U���f�[�^��Ԃ�
	AttackData GetAttackData(std::string& ownerName, std::string& attackName);
private:
	//�A�N�^�[��ǉ�
	void AddActor(std::shared_ptr<Actor> actor);
	//�ǉ��\��̃A�N�^�[������
	void CheckNextAddActors();
	//�폜�\��̃A�N�^�[���폜
	void CheckDeleteActors(const std::weak_ptr<Score> score);
	//�U���f�[�^�̍쐬
	void CreateAttackData();
	//�n���h�����[�h
	void LoadHandle();
	//�X�e�[�W�̏���
	void LoadStage(Stage::StageIndex index);
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
	//�U���f�[�^
	std::vector<AttackData> m_attackDatas;
	//UI�̎Q��
	std::weak_ptr<UIManager> m_uiManager;
private:
	//�n���h��
	 //�L�����N�^�[
	int m_playerHandle;
	int m_purpleDinosaurHandle;
	int m_smallDragonHandle;
	int m_bomberHandle;
	int m_bossDragonHandle;
	//�X�e�[�W�̃I�u�W�F�N�g
	int m_wallHandle;
	int m_pathHandle;
	int m_skyHandle;
	int m_blockGrassHandle; //���̒n��
	//�X�e�[�W�̓����蔻��
	int m_cubeHandle;
	int m_cylinderHandle;
	int m_planeHandle;
	//�A�C�e��
	int m_heartHandle;			//��
	int m_bombHandle;			//���e
	int m_ultGageUpHandle;		//�K�E�Q�[�W�A�b�v
	int m_attackUpHandle;		//�U���A�b�v
	int m_defenseUpHandle;		//�ϋv�A�b�v
};

