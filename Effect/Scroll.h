#pragma once

class Scroll
{
private: //�����o�ϐ�
	bool isScroll;
	float time;

public: //�����o�֐�
	Scroll();
	~Scroll() = default;

	// �X�N���[���̊J�n
	void ScrollStart();
	// �X�N���[���̍X�V
	void ScrollUpdate(float addTime);

	const bool GetFlag() const { return isScroll; }
	const float GetTime() const { return time; }
};
