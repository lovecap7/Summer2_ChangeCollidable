#pragma once
#include  <vector>
#include  <memory>
class UIBase;
class UIManager
{
public:
	UIManager();
	~UIManager();
	//“o˜^ˆ—
	void Entry(std::shared_ptr<UIBase>);
	//“o˜^‰ğœ
	void Exit(std::shared_ptr<UIBase> ui);
	//‰Šú‰»ˆ—
	void Init();
	//XVˆ—
	void Update();
	//•`‰æ
	void Draw()const;
private:
	//UI
	std::vector<std::shared_ptr<UIBase>> m_uis;
};

