#include <Dxlib.h>
#include "Input.h"
#include "Math/Vector2.h"

namespace
{
	//トリガーの押し込み具合
	constexpr float kTriggerPower = 128;
	//スティックの入力成立の大きさ
	constexpr int kLeftStickPowerX = 20;
	constexpr int kLeftStickPowerY = 20;
	//少しだけ倒していると判定する範囲
	constexpr float kLowPowerStickMin = 50.0f;
	constexpr float kLowPowerStickMax = 400.0f;
	//そこそこ倒していると判定する範囲
	constexpr float kMediumPowerStickMin = kLowPowerStickMax;
	constexpr float kMediumPowerStickMax = 700.0f;
	//最大まで倒していると判定する範囲
	constexpr float kHighPowerStickMin = kMediumPowerStickMax;
}

void Input::Init()
{
	m_inputActionMap["Ok"] = { {InputType::kKeyboard,KEY_INPUT_RETURN},{InputType::kPad,PAD_INPUT_A} };
	m_inputActionMap["Pause"] = { {InputType::kKeyboard,KEY_INPUT_P}, {InputType::kPad,PAD_INPUT_8} };
	m_inputActionMap["Start"] = { {InputType::kKeyboard,KEY_INPUT_P}, {InputType::kPad,PAD_INPUT_8} };
	m_inputActionMap["Select"] = { {InputType::kKeyboard,KEY_INPUT_TAB}, {InputType::kPad,PAD_INPUT_6} };
	m_inputActionMap["LS"] = { {InputType::kKeyboard,KEY_INPUT_LSHIFT}, {InputType::kPad,PAD_INPUT_START} };
	m_inputActionMap["RS"] = { {InputType::kKeyboard,KEY_INPUT_RSHIFT}, {InputType::kPad,PAD_INPUT_9} };
	m_inputActionMap["Cancel"] = { {InputType::kKeyboard,KEY_INPUT_ESCAPE}, {InputType::kPad,PAD_INPUT_B} };
	m_inputActionMap["Up"] = { {InputType::kKeyboard,KEY_INPUT_W}, {InputType::kPad,PAD_INPUT_UP} };
	m_inputActionMap["Down"] = { {InputType::kKeyboard,KEY_INPUT_S}, {InputType::kPad,PAD_INPUT_DOWN} };
	m_inputActionMap["Left"] = { {InputType::kKeyboard,KEY_INPUT_A}, {InputType::kPad,PAD_INPUT_LEFT} };
	m_inputActionMap["Right"] = { {InputType::kKeyboard,KEY_INPUT_D}, {InputType::kPad,PAD_INPUT_RIGHT} };
	m_inputActionMap["LB"] = { {InputType::kKeyboard,KEY_INPUT_J}, {InputType::kPad,PAD_INPUT_5} };
	m_inputActionMap["RB"] = { {InputType::kKeyboard,KEY_INPUT_L}, {InputType::kPad,PAD_INPUT_6} };
	m_inputActionMap["A"] = { {InputType::kKeyboard,KEY_INPUT_U}, {InputType::kPad,PAD_INPUT_1} };
	m_inputActionMap["B"] = { {InputType::kKeyboard,KEY_INPUT_H}, {InputType::kPad,PAD_INPUT_2} };
	m_inputActionMap["X"] = { {InputType::kKeyboard,KEY_INPUT_G}, {InputType::kPad,PAD_INPUT_3} };
	m_inputActionMap["Y"] = { {InputType::kKeyboard,KEY_INPUT_Y}, {InputType::kPad,PAD_INPUT_4} };

	//デバッグ用
	m_inputActionMap["SceneChange"] = { {InputType::kKeyboard,KEY_INPUT_1} };
	m_inputActionMap["StopUpdate"] = { {InputType::kKeyboard,KEY_INPUT_2} };
	m_inputActionMap["Enter"] = { {InputType::kKeyboard,KEY_INPUT_RETURN} };
	m_inputActionMap["Enter"] = { {InputType::kKeyboard,KEY_INPUT_RETURN} };
	m_inputActionMap["Max"] = { {InputType::kKeyboard,KEY_INPUT_M} ,{InputType::kPad,PAD_INPUT_5} };
	m_inputActionMap["BossArea"] = { {InputType::kKeyboard,KEY_INPUT_B}};
	m_inputActionMap["BossDead"] = { {InputType::kKeyboard,KEY_INPUT_N}};
}

void Input::Update()
{
	//前のフレームの入力情報を保存する
	m_lastInput = m_currentInput;

	//すべての入力を取得する
	char keyState[256] = {};
	int padState = {};
	int mouseState = {};
	GetHitKeyStateAll(keyState);
	padState = GetJoypadInputState(DX_INPUT_PAD1);
	mouseState = GetMouseInput();

	m_isBeforePressAny = m_isPressAny;
	m_isPressAny = false;
	//アクション名に割り当てられているすべてのキーの入力をチェックする
	for (const auto& keyInfo : m_inputActionMap)
	{
		bool isPress = false;
		for (const auto& inputInfo : keyInfo.second)
		{
			//キーボードのチェック
			if (inputInfo.type == InputType::kKeyboard && keyState[inputInfo.buttonID])
			{
				isPress = true;
			}
			//パッドのチェック
			if (inputInfo.type == InputType::kPad && padState & inputInfo.buttonID)
			{
				isPress = true;
			}
			//マウスのチェック
			if (inputInfo.type == InputType::kMouse && mouseState & inputInfo.buttonID)
			{
				isPress = true;
			}

			if (isPress)
			{
				m_isPressAny = isPress;
				break;
			}
		}
		//現在のフレームで押されていたかどうかを返す
		m_currentInput[keyInfo.first] = isPress;
	}

	//スティックの入力を初期化する
	m_stickInfo.leftStickX = 0;
	m_stickInfo.leftStickY = 0;
	m_stickInfo.rightStickX = 0;
	m_stickInfo.rightStickY = 0;

	//スティックの入力を取得する
	DxLib::GetJoypadAnalogInput(&m_stickInfo.leftStickX, &m_stickInfo.leftStickY, DX_INPUT_PAD1);
	DxLib::GetJoypadAnalogInputRight(&m_stickInfo.rightStickX, &m_stickInfo.rightStickY, DX_INPUT_PAD1);
	XINPUT_STATE* xInputState = new XINPUT_STATE;
	DxLib::GetJoypadXInputState(DX_INPUT_PAD1, xInputState);
	m_triggerInfo.left = xInputState->LeftTrigger;
	m_triggerInfo.right = xInputState->RightTrigger;
}

void Input::StopUpdate()
{
	for (auto& item : m_currentInput)
	{
		item.second = false;
	}
	for (auto& item : m_lastInput)
	{
		item.second = false;
	}

	m_stickInfo.leftStickX = 0;
	m_stickInfo.leftStickY = 0;
	m_stickInfo.rightStickX = 0;
	m_stickInfo.rightStickY = 0;

	m_triggerInfo.left = 0;
	m_triggerInfo.right = 0;
}

bool Input::IsPress(const std::string& action)const
{
	auto keyInfo = m_currentInput.find(action);

	//ボタン名が定義されていなかったらfalseを返す
	if (keyInfo == m_currentInput.end())
	{
		return false;
	}
	else
	{
		//見つかったらboolの値を返す
		return keyInfo->second;
	}
}

bool Input::IsTrigger(const std::string& action)const
{
	if (IsPress(action))
	{
		//前のフレームを参照
		auto last = m_lastInput.find(action);
		//未定義のボタン名が来たらfalseを返す
		if (last == m_lastInput.end())
		{
			return false;
		}
		else
		{
			//前のフレームでも押されていたら
			return !last->second;
		}
	}
	else
	{
		return false;
	}
}

bool Input::IsRelease(const std::string& action)const
{
	//まず押されているかを判定
	if (IsPress(action))
	{
		//押されていたらfalseを返す
		return false;
	}
	else
	{
		//前のフレームを参照
		auto last = m_lastInput.find(action);
		//押されていたらfalseを返す
		if (last == m_lastInput.end())
		{
			return false;
		}
		//押されていなければ
		else
		{
			return last->second;
		}
	}

	return false;
}

bool Input::IsLowPowerLeftStick()const
{
	//少しだけ倒しているならtrue
	Vector2 stickVec;
	stickVec.x = m_stickInfo.leftStickX;
	stickVec.y = m_stickInfo.leftStickY;
	float length = stickVec.Magnitude();
	if (length >= kLowPowerStickMin && length < kLowPowerStickMax)
	{
		return true;
	}
	return false;
}

bool Input::IsMediumPowerLeftStick()const
{
	//そこそこ倒しているならtrue
	Vector2 stickVec;
	stickVec.x = m_stickInfo.leftStickX;
	stickVec.y = m_stickInfo.leftStickY;
	float length = stickVec.Magnitude();
	if (length >= kMediumPowerStickMin && length < kMediumPowerStickMax)
	{
		return true;
	}
	return false;
}

bool Input::IsHighPowerLeftStick() const
{
	//倒し切ってるならtrue
	Vector2 stickVec;
	stickVec.x = m_stickInfo.leftStickX;
	stickVec.y = m_stickInfo.leftStickY;
	float length = stickVec.Magnitude();
	if (length >= kHighPowerStickMin)
	{
		return true;
	}
	return false;
}


bool Input::IsPushTrigger(bool right, int power)
{
	//トリガーの状態取得
	auto trigger = GetTriggerInfo();

	//右側を判定する場合
	if (right)
	{
		//引数よりも強く押し込まれたら
		if (trigger.right >= power)
		{
			return true;
		}
	}
	//左側を判定する場合
	else
	{
		//引数よりも強く押し込まれたら
		if (trigger.left >= power)
		{
			return true;
		}
	}
	return false;
}

bool Input::IsPushTrigger(bool right)
{
	return IsPushTrigger(right, kTriggerPower);
}

bool Input::IsPlessAny()
{
	return m_isPressAny;
}

bool Input::IsTriggerAny()
{
	if (m_isPressAny)
	{
		//前のフレーム押していなかったら
		if (m_isBeforePressAny != m_isPressAny)
		{
			return true;
		}
	}
	return false;
}

