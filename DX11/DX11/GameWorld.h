#pragma once
#include "GameObject.h"
#include "FBXSDKMeshLoader.h"
#include "InputOperatorCamera.h"
#include "Level.h"
#include <vector>
//前方宣言
class Camera;

/*
ゲームワールドのクラス
ワールド上に存在するすべてのゲームオブジェクトのポインタのリストを所持している。

複数のレベルをシーン上に読み込むことができるようにする必要がある

*/

/*
レベルは一つのワールドチャンクに相当するクラス

レベルは非同期で読み込みが可能であり、複数のレベルをシーン上に読み込めることが必要だ
だから非同期で破棄もできなければいけない、この時にどのゲームオブジェクトを破棄すればいいのか知るために
ゲームオブジェクトとレベルの紐づけは必要である

レベルのロードにはメモリの管理が問題になってくる
レベルの中で使うゲームオブジェクトがあらかじめ決まっていればあらかじめレベルをロードするメモリの総量がわかる
しかしレベル中に動的にゲームオブジェクトをロードしなければいけない場合もある、その場合はフラグメンテーションに気を付けなければいけない
レベル中で、使いまわせるゲームオブジェクトはなるべく使いまわすことで動的メモリのロードを防ぐことができる

レベルでは、レベル内のゲームオブジェクトを管理する



レベルクラスが持つべき情報は、シリアライズされたゲームオブジェクトデータ。ここでいうシリアライズされたデータとは


レベル内のゲームオブジェクトは必ずしもアセットとして存在しなくてもいい
レベル自身がゲームオブジェクトを保持する

*/

class GameWorld
{
public:

	//平行光源（Directonal Light）　ワールド空間で定義
	XMFLOAT4 m_vecLight = XMFLOAT4(-5.0f, -5.0f, 10.0f, 0.0f);

	//環境光　(0.0 ～ 1.0で指定）
	float m_Ambient = 0.0f;

	//ゲームオブジェクト
	GameObject* m_pBox;
	GameObject* m_pPlane;
	GameObject* m_pCamera;

	//ワールド内にロードされているレベルのポインタリスト
	std::vector<GameObject*> m_pLoadedLevelList;

	//ワールド内のすべてのゲームオブジェクトのポインタリスト
	std::vector<GameObject*> m_pGameObjectList;

	//カメラへのポインタ（このオブジェクトはカメラのゲームオブジェクトでコンポーネント属性として管理される、このクラスではメモリ管理を行わない）
	Camera* m_pCameraComponent;

	//入力を処理するクラス
	InputOperatorCamera* m_pInputOperatorCamera;

	//コンストラクタ、デストラクタ
	GameWorld();
	~GameWorld();

	//ゲームワールドにレベルをロードする、TODO:　レベルをロードするときにワールドのどの座用を基準に、どの回転で、ロードするのかのoffsetの指定が必要
	HRESULT LoadLevel(UINT levelID);

	//ロード済みレベルをアンロードする
	HRESULT DestroyLevel(UINT levelID);

	//シーンのフレーム処理
	void UpdateGameWorld();

	//シーンのレンダリング
	HRESULT RenderGameWorld();
};

