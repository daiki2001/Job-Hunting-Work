#pragma once
#include "./Header/Input.h"

class InputManager final
{
public: //シングルトン化
	static InputManager* Get();
private:
	InputManager();
	InputManager(const InputManager&) = delete;
	~InputManager() = default;
	InputManager operator=(const InputManager&) = delete;

private: //メンバ変数
	int mainUpKey;
	int mainDownKey;
	int mainLeftKey;
	int mainRightKey;
	int subUpKey;
	int subDownKey;
	int subLeftKey;
	int subRightKey;
	int decisionKey;

	DirectInput::GamepadInputType mainUp1Gamepad;
	DirectInput::GamepadInputType mainDown1Gamepad;
	DirectInput::GamepadInputType mainLeft1Gamepad;
	DirectInput::GamepadInputType mainRight1Gamepad;
	DirectInput::GamepadInputType mainUp2Gamepad;
	DirectInput::GamepadInputType mainDown2Gamepad;
	DirectInput::GamepadInputType mainLeft2Gamepad;
	DirectInput::GamepadInputType mainRight2Gamepad;
	DirectInput::GamepadInputType subUpGamepad;
	DirectInput::GamepadInputType subDownGamepad;
	DirectInput::GamepadInputType subLeftGamepad;
	DirectInput::GamepadInputType subRightGamepad;
	DirectInput::GamepadInputType decisionGamepad;

public: //メンバ関数
	const bool MainUp() const;
	const bool MainUpTrigger() const;
	const bool MainUpReturn() const;
	const bool MainDown() const;
	const bool MainDownTrigger() const;
	const bool MainDownReturn() const;
	const bool MainLeft() const;
	const bool MainLeftTrigger() const;
	const bool MainLeftReturn() const;
	const bool MainRight() const;
	const bool MainRightTrigger() const;
	const bool MainRightReturn() const;
	const bool SubUp() const;
	const bool SubUpTrigger() const;
	const bool SubUpReturn() const;
	const bool SubDown() const;
	const bool SubDownTrigger() const;
	const bool SubDownReturn() const;
	const bool SubLeft() const;
	const bool SubLeftTrigger() const;
	const bool SubLeftReturn() const;
	const bool SubRight() const;
	const bool SubRightTrigger() const;
	const bool SubRightReturn() const;
	const bool Decision() const;
	const bool DecisionTrigger() const;
	const bool DecisionReturn() const;
};
