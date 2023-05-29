#pragma once
#include "Particle.h"
#include <vector>
#include "./Header/EngineGeneral.h"

class SwitchParticle
{
private: //�G�C���A�X
	using Vector3 = Math::Vector3;

public: //�萔
	static const int MAX_TIME;

private: //�ÓI�����o�ϐ�
	static int model;

public: //�ÓI�����o�֐�
	// ����������
	static void Init();

private: //�����o�ϐ�
	std::vector<Particle> particle;

public: //�����o�֐�
	SwitchParticle();
	~SwitchParticle();

	// ��������
	void Create(const Vector3& startPos);
	// �X�V����
	void Update();
	// �`�揈��
	void Draw(const Vector3& offset = Vector3::Zero());
};
