#include <iostream>
#include "TinyCL.hpp"

int main()
{
	// GPU�̃f�o�C�X���擾����
	auto device = tcl::information.GetGPU();

	// �\�[�X�R�[�h�̃R���p�C��
	tcl::CLSource source("test.cl", "test", tcl::SourceType::Text);

	// �J�[�l�����s�N���X�̐���
	tcl::CLExecute exec(source, device);

	// �f�o�C�X�ɓn�����߂̔z���������
	const size_t N = 10;
	std::vector<float> input(N);
	for (int i = 0; i < N; ++i)
	{
		input[i] = i;
	}

	// �f�o�C�X���̃��������m��
	tcl::CLReadWriteBuffer x(exec, input);
	
	// ������s�������J�[�l���iGPU�N���X�^�j�̐���ݒ肷��
	// 1�����z��ŁC0����X�^�[�g���CN�̒����������Ă��āC�����N���Ƃɋ�؂�
	auto settings = tcl::CLWorkGroupSettings(1, { 0 }, { N }, { N }).Optimize(device);
	
	// ������ݒ肷��
	exec.SetArg(x());

	// �ݒ��n���Ď��s
	exec.Run(settings);

	// �f�o�C�X�̃���������C�z��֓ǂݏo��
	x.Read(input);

	// �����ɂ�
	for (int i = 0; i < N; ++i)
		std::cout << i << "," << input[i] << std::endl;

	// TODO: ���x������Ă݂���

	char a;
	std::cin >> a;
}