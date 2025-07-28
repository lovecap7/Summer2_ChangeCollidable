#pragma once
#include "../UIBase.h"
#include "../../../General/Math/MyMath.h"
#include <map>
#include <string>
class ResultScoreNameUI :
    public UIBase
{
public:
	ResultScoreNameUI();
	virtual ~ResultScoreNameUI();
	void Update() override;
	void Draw() const override;
private:
	//ƒnƒ“ƒhƒ‹
	std::map<std::string, int> m_handleMap;
};

