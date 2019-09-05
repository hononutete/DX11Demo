#include "FPSCounter.h"


FPSCounter::FPSCounter(UINT sampleCount)
{
	lastGetTime = (double)::timeGetTime();
	sampleListCount = sampleCount;
	sum = 0;
	//要素を0で初期化
	for (int i = 0; i < sampleListCount; i++)
		sampleList.push_back(0);
}


FPSCounter::~FPSCounter()
{
}

double FPSCounter::GetFPS()
{
	//先頭要素の値を取得し、それを引いた値を合計値としてキャッシュ
	double frontElem = sampleList.front();
	sum -= frontElem;

	//差分を取得
	double dif = CalcDif();


	//fpsを計算
	// FPS = 1秒間に何フレーム実行されたか
	// FPS = 1000ミリ秒（1秒）/ 1フレームの実行時間（ミリ秒）➡　この瞬間におけるFPS
	double fps = 1000 / dif;

	//現在のfpsを追加し、前のデータは削除
	sampleList.push_back(fps);
	sampleList.pop_front();

	//平均を計算
	double ave = (sum + fps) / sampleListCount;

	return ave;
}

void FPSCounter::UpdateSampleList()
{
	//先頭要素の値を取得し、それを引いた値を合計値としてキャッシュ
	double frontElem = sampleList.front();
	sum -= frontElem;

	//差分を取得
	double dif = CalcDif();
	

	//fpsを計算
	// FPS = 1秒間に何フレーム実行されたか
	// FPS = 1000ミリ秒（1秒）/ 1フレームの実行時間（ミリ秒）➡　この瞬間におけるFPS
	double fps = 1000 / dif;

	//現在のfpsを追加し、前のデータは削除
	sampleList.push_back(fps);
	sampleList.pop_front();

	//平均を計算
	double ave = (sum + fps) / sampleListCount;

}

double FPSCounter::CalcDif()
{
	//現在時刻を取得　単位はms（ミリ秒）
	DWORD curGetTime = ::timeGetTime();
	//差分取得
	double dif = (double)curGetTime - lastGetTime;
	//前回実行時の時間を記憶するバッファを現在時刻に更新
	lastGetTime = (double)curGetTime;

	return dif;

}