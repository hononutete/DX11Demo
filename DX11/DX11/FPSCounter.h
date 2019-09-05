#pragma once
#include <windows.h>
#include <MMSystem.h>
#include <list>

class FPSCounter
{
public:
	//FPS�T���v�����O�̂��߂̃L���[
	std::list<double> sampleList;
	UINT sampleListCount;

	//�O��̎��s���̎��Ԃ̃o�b�t�@
	double lastGetTime;

	//���v�l
	double sum;

	FPSCounter(UINT sampleCount);
	~FPSCounter();

	//���݂�FPS���擾����i�T���v�����O���ŕ��ω����ꂽFPS�j
	double GetFPS();

private:
	//FPS�T���v�����O�f�[�^�̍X�V
	void UpdateSampleList();

	//�O��̎��s����̌o�ߎ��Ԃ��擾
	double CalcDif();
};

