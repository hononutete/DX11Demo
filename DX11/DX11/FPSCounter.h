#pragma once
#include <windows.h>
#include <MMSystem.h>
#include <list>

class FPSCounter
{
public:
	//FPSサンプリングのためのキュー
	std::list<double> sampleList;
	UINT sampleListCount;

	//前回の実行時の時間のバッファ
	double lastGetTime;

	//合計値
	double sum;

	FPSCounter(UINT sampleCount);
	~FPSCounter();

	//現在のFPSを取得する（サンプリング数で平均化されたFPS）
	double GetFPS();

private:
	//FPSサンプリングデータの更新
	void UpdateSampleList();

	//前回の実行からの経過時間を取得
	double CalcDif();
};

