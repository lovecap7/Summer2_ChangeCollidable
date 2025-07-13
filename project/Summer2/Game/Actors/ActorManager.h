#pragma once
#include <list>
#include <memory>
#include "Attack/AttackBase.h"
#include "Item/ItemBase.h"
#include "../../General/CSVDataLoader.h"
#include "../../Scene/StageScene.h"
#include <map>

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
class CSVDataLoader;
class CharacterBase;
class ActorManager : public std::enable_shared_from_this<ActorManager>
{
public:
	ActorManager(Stage::StageIndex index,std::weak_ptr<UIManager> uiManager);
	~ActorManager();
	//�o�^
	void Entry(std::shared_ptr<Actor> actor);
	//����
	void Exit(std::shared_ptr<Actor> actor);
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
	std::weak_ptr<Player> GetPlayer() const { return m_player; };
	//�{�X���擾
	std::weak_ptr<EnemyBase> GetBoss() const { return m_boss; };
	//�{�X����
	std::weak_ptr<BossArea> GetBossArea() const { return m_bossArea; };
	//�v���C���[�ɋ߂��G���擾
	std::weak_ptr<Actor> GetNearestEnemy() const;
	//�U���f�[�^��Ԃ�
	AttackData GetAttackData(std::string& ownerName, std::string& attackName);
private:
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
	//CSV���[�h
	std::unique_ptr<CSVDataLoader> m_csvLoader;
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
	std::map<std::string, int> m_handles;
};

