#include "InputManager.h"

InputManager::InputManager() :
	mainUpKey(DIK_W),
	mainDownKey(DIK_S),
	mainLeftKey(DIK_A),
	mainRightKey(DIK_D),
	subUpKey(DIK_UP),
	subDownKey(DIK_DOWN),
	subLeftKey(DIK_LEFT),
	subRightKey(DIK_RIGHT),
	decisionKey(DIK_SPACE),
	mainUp1Gamepad(DirectInput::GamepadInputType::PAD_POV_UP),
	mainDown1Gamepad(DirectInput::GamepadInputType::PAD_POV_DOWN),
	mainLeft1Gamepad(DirectInput::GamepadInputType::PAD_POV_LEFT),
	mainRight1Gamepad(DirectInput::GamepadInputType::PAD_POV_RIGHT),
	mainUp2Gamepad(DirectInput::GamepadInputType::PAD_LSTICK_UP),
	mainDown2Gamepad(DirectInput::GamepadInputType::PAD_LSTICK_DOWN),
	mainLeft2Gamepad(DirectInput::GamepadInputType::PAD_LSTICK_LEFT),
	mainRight2Gamepad(DirectInput::GamepadInputType::PAD_LSTICK_RIGHT),
	subUpGamepad(DirectInput::GamepadInputType::PAD_RSTICK_UP),
	subDownGamepad(DirectInput::GamepadInputType::PAD_RSTICK_DOWN),
	subLeftGamepad(DirectInput::GamepadInputType::PAD_RSTICK_LEFT),
	subRightGamepad(DirectInput::GamepadInputType::PAD_RSTICK_RIGHT),
	decisionGamepad(DirectInput::GamepadInputType::PAD_BUTTON_A)
{
}

InputManager* InputManager::Get()
{
	static InputManager instance = {};
	return &instance;
}

const bool InputManager::MainUp() const
{
	return Input::IsKey(mainUpKey) ||
		Input::IsGamepad(mainUp1Gamepad) ||
		Input::IsGamepad(mainUp2Gamepad);
}

const bool InputManager::MainUpTrigger() const
{
	return Input::IsKeyTrigger(mainUpKey) ||
		Input::IsGamepadTrigger(mainUp1Gamepad) ||
		Input::IsGamepadTrigger(mainUp2Gamepad);
}

const bool InputManager::MainUpReturn() const
{
	return Input::IsKeyReturn(mainUpKey) ||
		Input::IsGamepadReturn(mainUp1Gamepad) ||
		Input::IsGamepadReturn(mainUp2Gamepad);
}

const bool InputManager::MainDown() const
{
	return Input::IsKey(mainDownKey) ||
		Input::IsGamepad(mainDown1Gamepad) ||
		Input::IsGamepad(mainDown2Gamepad);
}

const bool InputManager::MainDownTrigger() const
{
	return Input::IsKeyTrigger(mainDownKey) ||
		Input::IsGamepadTrigger(mainDown1Gamepad) ||
		Input::IsGamepadTrigger(mainDown2Gamepad);
}

const bool InputManager::MainDownReturn() const
{
	return Input::IsKeyReturn(mainDownKey) ||
		Input::IsGamepadReturn(mainDown1Gamepad) ||
		Input::IsGamepadReturn(mainDown2Gamepad);
}

const bool InputManager::MainLeft() const
{
	return Input::IsKey(mainLeftKey) ||
		Input::IsGamepad(mainLeft1Gamepad) ||
		Input::IsGamepad(mainLeft2Gamepad);
}

const bool InputManager::MainLeftTrigger() const
{
	return Input::IsKeyTrigger(mainLeftKey) ||
		Input::IsGamepadTrigger(mainLeft1Gamepad) ||
		Input::IsGamepadTrigger(mainLeft2Gamepad);
}

const bool InputManager::MainLeftReturn() const
{
	return Input::IsKeyReturn(mainLeftKey) ||
		Input::IsGamepadReturn(mainLeft1Gamepad) ||
		Input::IsGamepadReturn(mainLeft2Gamepad);
}

const bool InputManager::MainRight() const
{
	return Input::IsKey(mainRightKey) ||
		Input::IsGamepad(mainRight1Gamepad) ||
		Input::IsGamepad(mainRight2Gamepad);
}

const bool InputManager::MainRightTrigger() const
{
	return Input::IsKeyTrigger(mainRightKey) ||
		Input::IsGamepadTrigger(mainRight1Gamepad) ||
		Input::IsGamepadTrigger(mainRight2Gamepad);
}

const bool InputManager::MainRightReturn() const
{
	return Input::IsKeyReturn(mainRightKey) ||
		Input::IsGamepadReturn(mainRight1Gamepad) ||
		Input::IsGamepadReturn(mainRight2Gamepad);
}

const bool InputManager::SubUp() const
{
	return Input::IsKey(subUpKey) || Input::IsGamepad(subUpGamepad);
}

const bool InputManager::SubUpTrigger() const
{
	return Input::IsKeyTrigger(subUpKey) || Input::IsGamepadTrigger(subUpGamepad);
}

const bool InputManager::SubUpReturn() const
{
	return Input::IsKeyReturn(subUpKey) || Input::IsGamepadReturn(subUpGamepad);
}

const bool InputManager::SubDown() const
{
	return Input::IsKey(subDownKey) || Input::IsGamepad(subDownGamepad);
}

const bool InputManager::SubDownTrigger() const
{
	return Input::IsKeyTrigger(subDownKey) || Input::IsGamepadTrigger(subDownGamepad);
}

const bool InputManager::SubDownReturn() const
{
	return Input::IsKeyReturn(subDownKey) || Input::IsGamepadReturn(subDownGamepad);
}

const bool InputManager::SubLeft() const
{
	return Input::IsKey(subLeftKey) || Input::IsGamepad(subLeftGamepad);
}

const bool InputManager::SubLeftTrigger() const
{
	return Input::IsKeyTrigger(subLeftKey) || Input::IsGamepadTrigger(subLeftGamepad);
}

const bool InputManager::SubLeftReturn() const
{
	return Input::IsKeyReturn(subLeftKey) || Input::IsGamepadReturn(subLeftGamepad);
}

const bool InputManager::SubRight() const
{
	return Input::IsKey(subRightKey) || Input::IsGamepad(subRightGamepad);
}

const bool InputManager::SubRightTrigger() const
{
	return Input::IsKeyTrigger(subRightKey) || Input::IsGamepadTrigger(subRightGamepad);
}

const bool InputManager::SubRightReturn() const
{
	return Input::IsKeyReturn(subRightKey) || Input::IsGamepadReturn(subRightGamepad);
}

const bool InputManager::Decision() const
{
	return Input::IsKey(decisionKey) || Input::IsGamepad(decisionGamepad);
}

const bool InputManager::DecisionTrigger() const
{
	return Input::IsKeyTrigger(decisionKey) || Input::IsGamepadTrigger(decisionGamepad);
}

const bool InputManager::DecisionReturn() const
{
	return Input::IsKeyReturn(decisionKey) || Input::IsGamepadReturn(decisionGamepad);
}
