#pragma once
#include  <vector>
#include  <memory>
class UIBase;
class UIManager
{
public:
	UIManager();
	~UIManager();
	//�o�^����
	void Entry(std::shared_ptr<UIBase>);
	//�o�^����
	void Exit(std::shared_ptr<UIBase> ui);
	//����������
	void Init();
	//�X�V����
	void Update();
	//�`��
	void Draw()const;
private:
	//UI
	std::vector<std::shared_ptr<UIBase>> m_uis;
};

