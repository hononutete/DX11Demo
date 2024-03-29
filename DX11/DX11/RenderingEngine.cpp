#include "stdafx.h"
#include "RenderingEngine.h"


RenderingEngine::RenderingEngine()
{
}


RenderingEngine::~RenderingEngine()
{
}

void RenderingEngine::CreateMeshInstance() {

}

void RenderingEngine::RenderFrameAndSwapBuffer() {
	//ここではレンダリングを行う、シーンのレンダリングはステージに





	//ゲームオブジェクトのレンダリングの順序を決定する

	//マテリアルにレンダーキューをふる

	//透明オブジェクトに関しては、最後にレンダリングする必要がある

	//基本的なレンダリング順序
	//0: 背景などのスカイボックス
	//1: ゲームオブジェクト（より奥にあるオブジェクトからレンダリングしていく）
	//2:　透過オブジェクト

	//ここで考慮することは、カリングをどれだけやるかどうか
	//レンダリングする必要のないゲームオブジェクトをレンダリング対象から外す手法の種類
	//・オクルージョンカリング：遮蔽物に隠れているゲームオブジェクトをレンダリングしない
	//・背面カリング：裏向きのポリゴンを描画しないという一般的なカリング、DirectXの設定でON/OFFできる
	//・視錐台カリング：カメラの視錐台に含まれていないゲームオブジェクトをレンダリング対象から除外する
	//　視錐台カリングでは
}

//視錐台カリング
void RenderingEngine::FrustumCulling() {
	/*
	視錐台カリングでは、視錐台（カメラの範囲）の外側にあるゲームオブジェクトをレンダリング対象から除外する
	それぞれのゲームオブジェクトはバウンティングボリューム（基本的には球状のボリューム、一番計算が簡単だから、もしくはAABBなど）
	と視錐台平面の間で簡単なテストを行う
	*/	
}

void RenderingEngine::OcclusionCulling() {
	/*
	オクルージョンカリングとは、遮蔽物によって完全にレンダリングされないオブジェクトをレンダリング対象から除外することだ
	オクルージョンカリングでは、建物の中など遮蔽物が多い状況では非常に大事だが、遮蔽物がない状況では逆にオクルージョンカリングの負荷が高くなってしまう

	可視セット（Potentially Visible Set, PVS）をあらかじめきめる手法が有効だ
	PVSとはカメラのどんな視点に対しても見える可能性のあるゲームオブジェクトをリストにする

	PVSの作成のひとつの方法は、あらかじめレベルを領域に分割しておくこと
	・ユーザーが手動で設定したり、自動で設定するツールもある
	*/
}

void RenderingEngine::PortalCulling() {

}
