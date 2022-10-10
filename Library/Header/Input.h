#pragma once
#include "DirectInput.h"
#include "Win32API.h"

class Input final
{
public: // シングルトン化
	static Input* Get();
private:
	// コンストラクタをprivateにする
	Input() {}
	// コピーコンストラクタの削除
	Input(const Input& obj) = delete;
	// デストラクタをprivateにする
	~Input() {}
	// 代入演算子の削除
	const Input& operator=(const Input& obj) = delete;

private: // 静的メンバ変数
	// 入力デバイス
	static DirectInput* input;
	// 最新のキーボード情報用
	static char keys[256];
	// 1ループ(フレーム)前のキーボード情報
	static char oldKeys[256];
	// 最新のゲームパッド情報用
	static DIJOYSTATE gamepad;
	// 1ループ(フレーム)前のゲームパッド情報
	static DIJOYSTATE oldGamepad;

public: // 静的メンバ関数
	// 初期化
	static void Init();
	// 更新
	static void Update();
	// キーが押されているかどうか
	static bool IsKey(int key);
	// キーが押されたかどうか
	static bool IsKeyTrigger(int key);
	// キーから離れたかどうか
	static bool IsKeyReturn(int key);
	// キーが押されているかどうか
	static bool IsGamepad(const DirectInput::GamepadInputType& gamepadType);
	// キーが押されたかどうか
	static bool IsGamepadTrigger(const DirectInput::GamepadInputType& gamepadType);
	// キーから離れたかどうか
	static bool IsGamepadReturn(const DirectInput::GamepadInputType& gamepadType);
};
