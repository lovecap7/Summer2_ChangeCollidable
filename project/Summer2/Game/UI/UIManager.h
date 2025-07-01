#pragma once
#include <memory>
#include <list>
class UIBase;
class StageSetup;
class UIManager
{
public:
	UIManager();
	~UIManager();
	void Init(std::unique_ptr<StageSetup>& stageSetup);
	void Update();
	void Draw()const;
	void End();
	//UI’Ç‰Á
	void AddUI(std::shared_ptr<UIBase> ui);
private:
	//UI
	std::list<std::shared_ptr<UIBase>> m_uis;
	//íœ—\’è‚ÌUI‚ğíœ
	void CheckDelete();
};

